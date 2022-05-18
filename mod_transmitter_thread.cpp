#include "mod_transmitter_thread.h"
#include "usb_global.h"
#include <QElapsedTimer>

#include "crc16.h"
#include "srp_mod_protocol.h"

/* Extern global variables */
extern QWaitCondition modTransmitWakeUp;
extern QMutex m_mutex_mod;

extern float gain_data_float[2048];
extern float phase_data_float[2048];
extern uint16_t shift_for_qam_data_int;

/* Private variables */

/* Global variables */

ModTransmitterThread::ModTransmitterThread(QObject *parent) :
    QThread(parent)
{
}

ModTransmitterThread::~ModTransmitterThread()
{
    m_mutex_mod.lock();
    m_quit = true;
    modTransmitWakeUp.wakeOne();
    m_mutex_mod.unlock();

    wait();
}

void ModTransmitterThread::run()
{
    while(!m_quit)
    {
        m_mutex_mod.lock();
        modTransmitWakeUp.wait(&m_mutex_mod); // Wait condition unlocks mutex before 'wait', and will lock it again just after 'wait' is complete

        if(State == IDLE)
        {
            m_mutex_mod.unlock();
            continue;
        }

        if(retry)
        {
            retry = false;
            if(++n_attempts == n_MaxAttempts)
            {
                emit consolePutData(QString("ModTransmitterThread: error attempts exceed %1\n").arg(n_MaxAttempts), 1);
                n_attempts = 0;
                State = IDLE;
                m_mutex_mod.unlock();
                continue;
            }
        }
        else
        {
            n_attempts = 0;

            switch(State)
            {
                case TX_PHASE_TABLE_1:
                    if(++n_channel == n_elements/64)
                    {
                        emit consolePutData("Finished transmitting phase table, transmit gain table next\n", 1);
                        State = TX_GAIN_TABLE_2;
                        n_channel = 0;
                    }
                    break;

                case TX_GAIN_TABLE_2:
                    if(++n_channel == n_elements/64)
                    {
                        emit consolePutData("Finished transmitting gain table, transmit shift next\n", 1);
                        State = TX_SHIFT_3;
                    }
                    break;

                case TX_SHIFT_3:
                    emit consolePutData("Mod predistortion tables and 'QAM shift' transmission completed\n", 1);
                    State = IDLE;
                    m_mutex_mod.unlock();
                    continue;
                    break;

                 default:
                    emit consolePutData("ModTransmitterThread error wrong state\n", 1);
                    break;
            }
        }

        // Send next command
        switch(State)
        {
            case TX_PHASE_TABLE_1:
            case TX_GAIN_TABLE_2:
            {
                message.command = (State == TX_PHASE_TABLE_1) ? MessageBox::SET_PHASE_TABLE : MessageBox::SET_GAIN_TABLE;
                message.packet_adr = n_channel;

                uint32_t j = 0;
                for(uint32_t i = 0; i < 64; ++i)
                {
                    uint8_t *p_f;

                    if(State == TX_PHASE_TABLE_1)
                        p_f = reinterpret_cast<uint8_t*>(&phase_data_float[n_channel*64 + i]);
                    else //if(State == TRANSMIT_GAIN)
                        p_f = reinterpret_cast<uint8_t*>(&gain_data_float[n_channel*64 + i]);

                    message_box_buffer_mod[11 + j++] = p_f[0];
                    message_box_buffer_mod[11 + j++] = p_f[1];
                    message_box_buffer_mod[11 + j++] = p_f[2];
                    message_box_buffer_mod[11 + j++] = p_f[3];
                }
                break;
            }

            case TX_SHIFT_3:
                message.command = MessageBox::SET_SHIFT_QAM_DATA;
                message.packet_adr = 0;

//                //todo test
//                shift_for_qam_data_int = 4321;

                message_box_buffer_mod[11] = uint8_t(shift_for_qam_data_int & 0xFF);
                message_box_buffer_mod[12] = uint8_t((shift_for_qam_data_int >> 8) & 0xFF);
                break;

            default:
                emit consolePutData("ModTransmitterThread error wrong state\n", 1);
                break;
        }

        hs_data_received = false;

        uint16_t tx_len = MessageBox::message_header_to_array(&message, message_box_buffer_mod);
        postDataToStm32H7(message_box_buffer_mod, tx_len);

        // Start answer timeout
        emit startAnswerTimeoutTimer(timeoutAnswer_ms);
        m_mutex_mod.unlock();
    }
}

void ModTransmitterThread::ModStartTransmitPhaseGain()
{
    m_mutex_mod.lock();
    n_channel = 255;    //todo hack

//    //todo test float values
//    phase_data_float[0] = 0.1;
//    gain_data_float[0] = 0.1;
//    for(uint32_t i = 1; i < 2048; ++i)
//    {
//        phase_data_float[i] = phase_data_float[i - 1] + 0.1;
//        gain_data_float[i] = gain_data_float[i - 1] + 0.2;
//    }

    message.command = MessageBox::SET_PHASE_TABLE;
    message.message_id = 0;
    message.master_address = MessageBox::MOD_ADDR;
    message.own_address = MessageBox::MASTER_ADDR;
    message.data_len = 256;

    State = TX_PHASE_TABLE_1;

    retry = false;
    n_attempts = 0;
    m_mutex_mod.unlock();

    modTransmitWakeUp.wakeOne();
}

bool ModTransmitterThread::ModAnswerReceived(const uint8_t *p_data, int length)
{
    if(State == IDLE)
        return false;

    if(length < 12)
    {
        emit consolePutData(QString("ModAnswerReceived error length is too short %1\n").arg(length), 1);
        return false;
    }

    // p_data[0] : addr
    // p_data[1] : command
    // p_data[2] : message_id
    // p_data[3] : 0x01
    // p_data[4] [5] : data_len
    // p_data[6] [7] [8] [9] - channel/packet_addr

    if(p_data[0] != MessageBox::MOD_ADDR)
    {
        emit consolePutData(QString("ModAnswerReceived warning, answer addr != MOD_ADDR\n"), 1);
        return false;
    }

    // Check crc
    uint16_t crc = ((uint16_t)p_data[length - 1] << 8)
                   | (uint16_t)p_data[length - 2];

    bool res = check_crc16(p_data, length - 2, crc);

    if(!res)
    {
        emit consolePutData("ModAnswerReceived crc error\n", 1);
        return false;
    }

    switch(State)
    {
        default:
            emit consolePutData("ModAnswerReceived warning wrong state\n", 1);
            break;

        case TX_PHASE_TABLE_1:
            if(p_data[1] == MessageBox::SET_PHASE_TABLE)
            {
                emit stopAnswerTimeoutTimer();
                modTransmitWakeUp.wakeOne();
            }
            else
            {
                emit consolePutData("ModAnswerReceived answer wrong command\n", 1);
                return false;
            }
            break;

        case TX_GAIN_TABLE_2:
            if(p_data[1] == MessageBox::SET_GAIN_TABLE)
            {
                emit stopAnswerTimeoutTimer();
                modTransmitWakeUp.wakeOne();
            }
            else
            {
                emit consolePutData("ModAnswerReceived answer wrong command\n", 1);
                return false;
            }
            break;

        case TX_SHIFT_3:
            if(p_data[1] == MessageBox::SET_SHIFT_QAM_DATA)
            {
                emit stopAnswerTimeoutTimer();
                modTransmitWakeUp.wakeOne();
            }
            else
            {
                emit consolePutData("ModAnswerReceived answer wrong command\n", 1);
                return false;
            }
            break;
    }

    return true;
}

void ModTransmitterThread::ModAnswerDataReceived()
{
    m_mutex_mod.lock();
    if(State != IDLE)
        hs_data_received = true;
    m_mutex_mod.unlock();
}

void ModTransmitterThread::timeoutAnswer()
{
    emit consolePutData("Mod answer timeout\n", 1);

    m_mutex_mod.lock();
    if(State != IDLE)
    {
        if(!hs_data_received)
        {
            emit consolePutData("Retry previous transfer\n", 1);
            retry = true;
            m_mutex_mod.unlock();
            modTransmitWakeUp.wakeOne();
            return;
        }

        // Some HS data (probably broken) received
        // Assume it is an answer from MOD
        emit consolePutData("Some HS data was received, assume it was a good answer from MOD\n", 1);
        hs_data_received = false;
        m_mutex_mod.unlock();
        modTransmitWakeUp.wakeOne();
        return;
    }

    m_mutex_mod.unlock();
}
