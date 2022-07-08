#include "mod_transmitter_thread.h"
#include "usb_global.h"
#include <QElapsedTimer>

#include "crc16.h"
#include "srp_mod_protocol.h"
#include "../SRP_HS_USB_PROTOCOL/SRP_HS_USB_Protocol.h"
#include "global_vars.h"
#include "ringbuffer.h"

/* Extern global variables */
extern QWaitCondition modTransmitWakeUp;
extern QMutex m_mutex_mod;

extern float gain_data_float[2048];
extern float phase_data_float[2048];
extern uint16_t shift_for_qam_data_int;

extern RingBuffer *m_ring;

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

void ModTransmitterThread::setState(TState state)
{
    State = state;
    emit consolePutData(QString(":: Predistortion auto cfg :: State #%1\n").arg(State), 1);
    emit setStatus(State);
}

void ModTransmitterThread::run()
{
    static uint32_t n_commands = 0;
    static uint32_t Timeout = 0;

    while(!m_quit)
    {
        m_mutex_mod.lock();
        modTransmitWakeUp.wait(&m_mutex_mod); // Wait condition unlocks mutex before 'wait', and will lock it again just after 'wait' is complete

        switch(State)
        {
            case IDLE:
            case ERROR_AGC_MODSTAT:
            case ERROR_SWEEP_TIMEOUT:
            case ERROR_AGC_SWEEP:
            case ERROR_FREQ_ESTIMATE_TIMEOUT:
            case ERROR_AGC_SIN35KHZ:
                m_mutex_mod.unlock();
                continue;

            case AUTOCFG_START:

                // Disable ring buffer for QAM decoder (disable QAM decoder)
                emit consolePutData(":: Predistortion auto cfg :: disable QAM decoder ring buffer'\n", 1);
                m_ring->SetActive(false);

                // Send 'AGC start' to STM32
                emit consolePutData(":: Predistortion auto cfg :: send 'AGC start'\n", 1);          
                emit sendCommandToSTM32(USB_CMD_AGC_START, nullptr, 0);

                // Delay
                QThread::msleep(100);

                setState(SIN35KHZ_MOD_COMMANDS_FOR_AGC);
                n_commands = 0;
                Set_AGC_State(AGC_START);
                /* fallthrough */

            case SIN35KHZ_MOD_COMMANDS_FOR_AGC:
            case SIN35KHZ_MOD_COMMANDS_FOR_AGC_2:
                {
                    // Send 'SEND_SIN_35KHZ' command to MOD
                    uint8_t AGCState = Get_AGC_State();

                    // Check for continuous AGC errors
                    if(AGCState == AGC_ERROR)
                    {
                        if(++n_continuous_errors > n_MaxContinuousAgcErrors)
                        {
                            n_continuous_errors = 0;
                            emit consolePutData(":: Predistortion auto cfg :: AGC failed with status 'AGC error' (too much continuous errors)\n", 1);
                            setState(ERROR_AGC_SIN35KHZ);
                            calculatePredistortionTablesStop();
                            break;
                        }
                    }
                    else
                        n_continuous_errors = 0;

                    // At least one 'SIN 35kHz' must be transmitted and AGCState = AGC_OK
                    if(AGCState != AGC_OK || !n_commands)
                    {                       
                        if(++n_commands > n_MaxSin35kHzCommands)
                        {
                            emit consolePutData(":: Predistortion auto cfg :: AGC for 'SIN 35kHz' error: too many 'SEND_SIN_35KHZ' commands transmitted to MOD and still no 'AGC OK' status from STM32\n", 1);
                            setState(ERROR_AGC_SIN35KHZ);
                            calculatePredistortionTablesStop();
                            break;
                        }

                        message.command = CMessageBox::SEND_SIN_35KHZ;
                        message.packet_adr = 0;
                        message.data_len = 0;
                        message.message_id = 3;//0;
                        message.master_address = CMessageBox::MOD_ADDR;
                        message.own_address = CMessageBox::MASTER_ADDR;

                        uint16_t tx_len = CMessageBox::message_header_to_array(&message, message_box_buffer_mod);
                        emit consolePutData(QString(":: Predistortion auto cfg :: Send 'SEND_SIN_35KHZ' command to MOD #%1 of max #%2\n").arg(n_commands).arg(n_MaxSin35kHzCommands), 1);
                        postDataToStm32H7(message_box_buffer_mod, tx_len);

                        // Start timeout before next command
                        emit startAnswerTimeoutTimer(timeoutAgcSin35kHzCommands_ms);
                        break;
                    }

                    emit consolePutData(":: Predistortion auto cfg :: AGC for 'SIN 35kHz' configured, state AGC_OK\n", 1);

                    if(State == SIN35KHZ_MOD_COMMANDS_FOR_AGC_2)
                    {
                        setState(START_TX_PREDISTORTION_TABLES_TO_MOD);
                        emit startAnswerTimeoutTimer(100);
                        break;
                    }

                    setState(ADC_START_FOR_SIN600);
                }
                /* fallthrough */

            case ADC_START_FOR_SIN600:
            {
                // Send 'ADC_START for SIN 600' to STM32
                emit consolePutData(":: Predistortion auto cfg :: send 'ADC_START for SIN 600'\n", 1);
                Set_Special_Command_SIN600(true);

                uint32_t adc_data_length = 31400;
                emit sendCommandToSTM32(USB_CMD_ADC_START, (uint8_t*)&adc_data_length, 4);

                // Next state
                setState(SIN600_MOD_COMMAND);

                // Delay
                QThread::msleep(100);
            }
                /* fallthrough */

            case SIN600_MOD_COMMAND:
            {
                emit consolePutData(QString(":: Predistortion auto cfg :: Send 'SEND_SIN_35KHZ_600' command to MOD\n"), 1);
                Set_FreqEstState(FREQ_EST_START);

                message.command = CMessageBox::SEND_SIN_35KHZ_600;
                message.packet_adr = 0;
                message.data_len = 0;
                message.message_id = 0;
                message.master_address = CMessageBox::MOD_ADDR;
                message.own_address = CMessageBox::MASTER_ADDR;

                uint16_t tx_len = CMessageBox::message_header_to_array(&message, message_box_buffer_mod);
                postDataToStm32H7(message_box_buffer_mod, tx_len);

                // Wait for freq estimate
                QThread::msleep(100);

                // Next state
                State = FREQ_ESTIMATE_FUNC_WAIT;

                // Reset timeout
                Timeout = 0;

                // Start timeout to check 'frequency estimate' function completion
                emit startAnswerTimeoutTimer(100);
            }
                break;

            case FREQ_ESTIMATE_FUNC_WAIT:
                {
                    ++Timeout;

                    TFreqEstState s = Get_FreqEstState();

                    if(s == FREQ_EST_COMPLETE)
                    {
                        // Complete, go to next state
                        setState(AGC_START_FOR_SWEEP);
                    }
                    else if(Timeout >= 30000/100)  // 30 sec
                    {
                        emit consolePutData(":: Predistortion auto cfg :: Error freq estimate timeout elapsed\n", 1);
                        setState(ERROR_FREQ_ESTIMATE_TIMEOUT);
                        calculatePredistortionTablesStop();
                        break;
                    }
                    else
                    {
                        // Start timeout to check 'frequency estimate' function completion
                        emit startAnswerTimeoutTimer(100);
                        break;
                    }
                }
                /* fallthrough */

            case AGC_START_FOR_SWEEP:
                // Send 'AGC start' to STM32
                emit consolePutData(":: Predistortion auto cfg :: send 'AGC start'\n", 1);
                emit sendCommandToSTM32(USB_CMD_AGC_START, nullptr, 0);

                // Delay
                QThread::msleep(100);

                setState(SWEEP_MOD_COMMANDS_FOR_AGC);
                n_commands = 0;
                Set_AGC_State(AGC_START);
                /* fallthrough */

            case SWEEP_MOD_COMMANDS_FOR_AGC:
                {
                    // 2. Send 'SWEEP' command to MOD
                    uint8_t AGCState = Get_AGC_State();

                    // At least one 'SWEEP' must be transmitted and AGCState = AGC_OK
                    if(AGCState != AGC_OK || !n_commands)
                    {
                        if(++n_commands > n_MaxSweepCommands)
                        {
                            emit consolePutData(":: Predistortion auto cfg :: AGC for 'SWEEP' error: too many 'SEND_SWEEP_SIGNAL' commands transmitted to MOD and still no 'AGC OK' status from STM32\n", 1);
                            setState(ERROR_AGC_SWEEP);
                            calculatePredistortionTablesStop();
                            break;
                        }

                        message.command = CMessageBox::SEND_SWEEP_SIGNAL;
                        message.packet_adr = 0;
                        message.data_len = 0;
                        message.message_id = 0;
                        message.master_address = CMessageBox::MOD_ADDR;
                        message.own_address = CMessageBox::MASTER_ADDR;

                        uint16_t tx_len = CMessageBox::message_header_to_array(&message, message_box_buffer_mod);
                        emit consolePutData(QString(":: Predistortion auto cfg :: Send 'SEND_SWEEP_SIGNAL' command to MOD #%1 of max #%2\n").arg(n_commands).arg(n_MaxSweepCommands), 1);
                        postDataToStm32H7(message_box_buffer_mod, tx_len);

                        // Start timeout before next command
                        emit startAnswerTimeoutTimer(timeoutAgcSweepCommands_ms);
                        break;
                    }

                    emit consolePutData(":: Predistortion auto cfg :: AGC for 'SIN 35kHz' configured, state AGC_OK\n", 1);
                    setState(ADC_START_FOR_SWEEP);
                }
                /* fallthrough */

            case ADC_START_FOR_SWEEP:
                {
                    // 3. Send 'ADC_START for SWEEP' to STM32
                    emit consolePutData("Predistortion auto cfg: send 'ADC_START for SIN 600'\n", 1);
                    Set_Special_Command_Sweep(true);

                    uint32_t adc_data_length = 440000;
                    emit sendCommandToSTM32(USB_CMD_ADC_START, (uint8_t*)&adc_data_length, 4);

                    // Next state
                    setState(SWEEP_MOD_COMMAND);

                    // Delay
                    QThread::msleep(100);
                }
                /* fallthrough */

            case SWEEP_MOD_COMMAND:
                {
                    Set_SweepState(SWEEP_START);    //todo

                    message.command = CMessageBox::SEND_SWEEP_SIGNAL;
                    message.packet_adr = 0;
                    message.data_len = 0;
                    message.message_id = 0;
                    message.master_address = CMessageBox::MOD_ADDR;
                    message.own_address = CMessageBox::MASTER_ADDR;

                    uint16_t tx_len = CMessageBox::message_header_to_array(&message, message_box_buffer_mod);
                    emit consolePutData(QString(":: Predistortion auto cfg :: Send 'SEND_SIN_35KHZ_600Z' command to MOD\n"), 1);
                    postDataToStm32H7(message_box_buffer_mod, tx_len);

                    // Wait for freq estimate
                    QThread::msleep(100);

                    // Next state
                    setState(SWEEP_FUNC_WAIT);

                    // Reset timeout
                    Timeout = 0;

                    // Start timeout to check 'frequency estimate' function completion
                    emit startAnswerTimeoutTimer(100);
                }
                /* fallthrough */

            case SWEEP_FUNC_WAIT:
                {
                    ++Timeout;

                    TSweepState s = Get_SweepState();

                    if(s == SWEEP_COMPLETE)
                    {
                        // Complete, go to next state
                        setState(AGC_START_FOR_SIN35KHZ);
                    }
                    else if(Timeout >= 30000/100)  // 30 sec
                    {
                        emit consolePutData(":: Predistortion auto cfg :: Error predistortion auto cfg: sweep func timeout elapsed\n", 1);
                        setState(ERROR_SWEEP_TIMEOUT);
                        calculatePredistortionTablesStop();
                        break;
                    }
                    else
                    {
                        // Start timeout to check 'sweep' function completion
                        emit startAnswerTimeoutTimer(100);
                        break;
                    }
                }
                /* fallthrough */

            case AGC_START_FOR_SIN35KHZ:
                // Send 'AGC start' to STM32
                emit consolePutData(":: Predistortion auto cfg :: send 'AGC start'\n", 1);
                emit sendCommandToSTM32(USB_CMD_AGC_START, nullptr, 0);

                // Delay
                QThread::msleep(100);

                setState(SIN35KHZ_MOD_COMMANDS_FOR_AGC_2);
                n_commands = 0;
                Set_AGC_State(AGC_START);

                // Start timeout before sending commands to MOD
                emit startAnswerTimeoutTimer(100);
                break;

            case START_TX_PREDISTORTION_TABLES_TO_MOD:
                // Start tranmitting predistortion tables to MOD
                message.command = CMessageBox::SET_PHASE_TABLE;
                message.message_id = 0;
                message.master_address = CMessageBox::MOD_ADDR;
                message.own_address = CMessageBox::MASTER_ADDR;
                message.data_len = 256;

                StatePredistTx = TX_START;

                retry = false;
                n_attempts = 0;
                n_attempts_high_level = 0;

                setState(TX_PREDISTORTION_TABLES_TO_MOD);
                /* fallthrough */

             case TX_PREDISTORTION_TABLES_TO_MOD:
                transmitPredistortionTables();
                break;

            case AGC_START_FOR_MOD_STAT:
                // Send 'AGC start' to STM32
                emit consolePutData(":: Predistortion auto cfg :: send 'AGC start'\n", 1);
                emit sendCommandToSTM32(USB_CMD_AGC_START, nullptr, 0);

                // Delay
                QThread::msleep(100);

                setState(STAT_MOD_COMMANDS_FOR_AGC);
                n_commands = 0;
                Set_AGC_State(AGC_START);
                /* fallthrough */

             case STAT_MOD_COMMANDS_FOR_AGC:
                {
                    // Send 'GET_STATUS' command to MOD
                    uint8_t AGCState = Get_AGC_State();

                    // At least one 'GET STATUS' must be transmitted and AGCState = AGC_OK
                    if(AGCState != AGC_OK || !n_commands)
                    {
                        if(++n_commands > n_MaxModStatusCommands)
                        {
                            emit consolePutData(":: Predistortion auto cfg :: AGC for 'MOD STATUS' error: too many 'GET STATUS' commands transmitted to MOD and still no 'AGC OK' status from STM32\n", 1);
                            setState(ERROR_AGC_MODSTAT);
                            calculatePredistortionTablesStop();
                            break;
                        }

                        message.command = CMessageBox::STATUS;
                        message.packet_adr = 0;
                        message.data_len = 0;
                        message.message_id = 0;
                        message.master_address = CMessageBox::MOD_ADDR;
                        message.own_address = CMessageBox::MASTER_ADDR;

                        uint16_t tx_len = CMessageBox::message_header_to_array(&message, message_box_buffer_mod);
                        emit consolePutData(QString(":: Predistortion auto cfg :: Send 'GET STATUS' command to MOD #%1 of max #%2\n").arg(n_commands).arg(n_MaxModStatusCommands), 1);
                        postDataToStm32H7(message_box_buffer_mod, tx_len);

                        // Start timeout before next command
                        emit startAnswerTimeoutTimer(timeoutModStatusCommands_ms);
                        break;
                    }

                    emit consolePutData(":: Predistortion auto cfg :: AGC for 'GET STATUS' configured, state AGC_OK\n", 1);

                    // Finishing
                    setState(AUTOCFG_COMPLETE_SUCCESSFULLY);
                    emit startAnswerTimeoutTimer(100);
                }
                break;

             case AUTOCFG_COMPLETE_SUCCESSFULLY:
                emit consolePutData(":: Predistortion auto cfg :: auto configuration complete, all operations completed successfully\n", 1);
                calculatePredistortionTablesStop();
                break;

             default:
                emit consolePutData(":: Predistortion auto cfg :: error wrong state\n", 1);
                break;
        }

        m_mutex_mod.unlock();
    }
}

void ModTransmitterThread::ModStartTransmitPhaseGain()
{
    m_mutex_mod.lock();

    if(State != IDLE
       && State != AUTOCFG_COMPLETE_SUCCESSFULLY
       && State != ERROR_AGC_MODSTAT
       && State != ERROR_SWEEP_TIMEOUT
       && State != ERROR_AGC_SWEEP
       && State != ERROR_FREQ_ESTIMATE_TIMEOUT
       && State != ERROR_AGC_SIN35KHZ)
    {
        emit consolePutData("Error: unable to start transmitting phase & gain tables, state != IDLE (already transmitting?)\n", 1);
        m_mutex_mod.unlock();
        return;
    }

    setState(START_TX_PREDISTORTION_TABLES_TO_MOD);

    m_mutex_mod.unlock();
    modTransmitWakeUp.wakeOne();
}

void ModTransmitterThread::transmitPredistortionTables()
{
    if(retry)
    {
        retry = false;
        if(++n_attempts == n_MaxAttempts)
        {
            n_attempts = 0;

            if(++n_attempts_high_level > n_MaxAttemptsHighLevel)
            {
                emit consolePutData(":: Predistortion auto cfg :: failed to transmit predistorion tables to MOD\n", 1);
                setState(ERROR_PREDISTORTION_TABLES_TX_FAILED);
                calculatePredistortionTablesStop();
                return;
            }

            emit consolePutData(QString(":: Predistortion auto cfg :: error attempts exceeded %1, start again from first command\n").arg(n_MaxAttempts), 1);
            emit consolePutData(":: Predistortion auto cfg :: Starting transmit 'phase table' \n", 1);
            StatePredistTx = TX_PHASE_TABLE;
            n_channel = 0;
        }
    }
    else
    {
        n_attempts = 0;

        switch(StatePredistTx)
        {
            case TX_START:
                emit consolePutData(QString(":: Predistortion auto cfg :: Starting transmit predistortion tables, attempt %1\n").arg(n_attempts_high_level), 1);
                emit consolePutData(":: Predistortion auto cfg :: Starting transmit 'phase table' \n", 1);

                StatePredistTx = TX_PHASE_TABLE;
                n_channel = 0;
                break;

            case TX_PHASE_TABLE:
                if(++n_channel == n_elements/64)
                {
                    emit consolePutData(":: Predistortion auto cfg :: Finished transmitting 'phase table', transmit 'gain table' next\n", 1);
                    StatePredistTx = TX_GAIN_TABLE;
                    n_channel = 0;
                }
                break;

            case TX_GAIN_TABLE:
                if(++n_channel == n_elements/64)
                {
                    emit consolePutData(":: Predistortion auto cfg :: Finished transmitting 'gain table', transmit 'shift + crc' next\n", 1);
                    StatePredistTx = TX_SHIFT_CRC;
                }
                break;

            case TX_SHIFT_CRC:
                emit consolePutData(":: Predistortion auto cfg :: Mod predistortion tables and 'shift + crc' transmission completed\n", 1);
                StatePredistTx = TX_FINISHED;

                // Start AGC for final configuration
                setState(AGC_START_FOR_MOD_STAT);

                // Wait some time before AGC, so MOD could apply new predistortion tables
                emit startAnswerTimeoutTimer(500);
                return;

             default:
                emit consolePutData(":: Predistortion auto cfg :: error wrong state\n", 1);
                break;
        }
    }

    // Send next command
    switch(StatePredistTx)
    {
        case TX_PHASE_TABLE:
        case TX_GAIN_TABLE:
        {
            message.command = (StatePredistTx == TX_PHASE_TABLE) ? CMessageBox::SET_PHASE_TABLE : CMessageBox::SET_GAIN_TABLE;
            message.packet_adr = n_channel;

            uint8_t *p_f;
            uint32_t j = 0;

            for(uint32_t i = 0; i < 64; ++i)
            {
                if(StatePredistTx == TX_PHASE_TABLE)
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

        case TX_SHIFT_CRC:
        {
            message.command = CMessageBox::SET_SHIFT_QAM_DATA;
            message.packet_adr = 0;

            message_box_buffer_mod[11] = uint8_t(shift_for_qam_data_int & 0xFF);
            message_box_buffer_mod[12] = uint8_t((shift_for_qam_data_int >> 8) & 0xFF);

            // Calculate crc16 for phase & gain tables and shift
            calc_crc16((uint8_t*)&phase_data_float, sizeof(phase_data_float));
            calc_crc16_continue((uint8_t*)&gain_data_float, sizeof(phase_data_float));
            uint16_t crc16 = calc_crc16_continue((uint8_t*)&shift_for_qam_data_int, sizeof(shift_for_qam_data_int));

            message_box_buffer_mod[13] = uint8_t(crc16 & 0xFF);
            message_box_buffer_mod[14] = uint8_t((crc16 >> 8) & 0xFF);
        }
            break;

        default:
            emit consolePutData(":: Predistortion auto cfg :: error wrong state\n", 1);
            break;
    }

    hs_data_received = false;

    uint16_t tx_len = CMessageBox::message_header_to_array(&message, message_box_buffer_mod);
    postDataToStm32H7(message_box_buffer_mod, tx_len);

    // Start answer timeout
    emit startAnswerTimeoutTimer(timeoutAnswer_ms);
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
//    emit consolePutData("Mod answer timeout\n", 1);

    m_mutex_mod.lock();

    if(State == IDLE)
    {
        m_mutex_mod.unlock();
        return;
    }

    else if(State == TX_PREDISTORTION_TABLES_TO_MOD)
    {
        if(!hs_data_received)
        {
            emit consolePutData(":: Predistortion auto cfg :: retry previous transfer\n", 1);
            retry = true;
        }

        // Some HS data (probably broken) received
        // Assume it is an answer from MOD
        emit consolePutData(":: Predistortion auto cfg :: some HS data was received, assume it was a good answer from MOD\n", 1);
        hs_data_received = false;
    }

    m_mutex_mod.unlock();

    // Get back to thread run()
    modTransmitWakeUp.wakeOne();
}

// Public
void ModTransmitterThread::calculatePredistortionTablesStart()
{
    m_AutoConfigurationMode = true;

    emit consolePutData("==================================================\n"
                        "Starting predistortion auto configuration sequence\n"
                        "==================================================\n", 1);
    m_mutex_mod.lock();
    setState(AUTOCFG_START);
    StatePredistTx = TX_IDLE;
    m_mutex_mod.unlock();
    modTransmitWakeUp.wakeOne();
}

// Private
void ModTransmitterThread::calculatePredistortionTablesStop()
{
    // Check AGC state, stop AGC if needed
    uint8_t n_retrys = 2;

    while(1)
    {
        if(Get_AGC_State() == AGC_OK)
            break;

        if(!n_retrys)
        {
            emit consolePutData(":: Predistortion auto cfg :: error 'AGC stop' failed\n", 1);
            break;
        }

        emit consolePutData(":: Predistortion auto cfg :: send 'AGC stop'\n", 1);
        emit sendCommandToSTM32(USB_CMD_AGC_STOP, nullptr, 0);
        QThread::msleep(1000);
        emit sendCommandToSTM32(USB_CMD_GET_STATUS, nullptr, 0);
        QThread::msleep(1000);

        --n_retrys;
    }

    StatePredistTx = TX_IDLE;
    emit consolePutData(":: Predistortion auto cfg :: enable QAM decoder ring buffer'\n", 1);

    // Unlock ring buffer for QAM decoder (enable QAM decoder)
    m_ring->SetActive(true);

    // QAM decoder set first pass flag
    emit qamDecoderReset();

    m_AutoConfigurationMode = false;
}

// Parse received data from MOD, for future use
bool ModTransmitterThread::ModAnswerReceived(const uint8_t *p_data, int length)
{
    Q_UNUSED(p_data);
    Q_UNUSED(length);

//    if(State == IDLE)
//        return false;

//    if(length < 12)
//    {
//        emit consolePutData(QString("ModAnswerReceived error length is too short %1\n").arg(length), 1);
//        return false;
//    }

//    // p_data[0] : addr
//    // p_data[1] : command
//    // p_data[2] : message_id
//    // p_data[3] : 0x01
//    // p_data[4] [5] : data_len
//    // p_data[6] [7] [8] [9] - channel/packet_addr

//    if(p_data[0] != MessageBox::MOD_ADDR)
//    {
//        emit consolePutData(QString("ModAnswerReceived warning, answer addr != MOD_ADDR\n"), 1);
//        return false;
//    }

//    // Check crc
//    uint16_t crc = ((uint16_t)p_data[length - 1] << 8)
//                   | (uint16_t)p_data[length - 2];

//    bool res = check_crc16(p_data, length - 2, crc);

//    if(!res)
//    {
//        emit consolePutData("ModAnswerReceived crc error\n", 1);
//        return false;
//    }

//    switch(State)
//    {
//        default:
//            emit consolePutData("ModAnswerReceived warning wrong state\n", 1);
//            break;

//        case TX_PHASE_TABLE_1:
//            if(p_data[1] == MessageBox::SET_PHASE_TABLE)
//            {
//                emit stopAnswerTimeoutTimer();
//                modTransmitWakeUp.wakeOne();
//            }
//            else
//            {
//                emit consolePutData("ModAnswerReceived answer wrong command\n", 1);
//                return false;
//            }
//            break;

//        case TX_GAIN_TABLE_2:
//            if(p_data[1] == MessageBox::SET_GAIN_TABLE)
//            {
//                emit stopAnswerTimeoutTimer();
//                modTransmitWakeUp.wakeOne();
//            }
//            else
//            {
//                emit consolePutData("ModAnswerReceived answer wrong command\n", 1);
//                return false;
//            }
//            break;

//        case TX_SHIFT_3:
//            if(p_data[1] == MessageBox::SET_SHIFT_QAM_DATA)
//            {
//                emit stopAnswerTimeoutTimer();
//                modTransmitWakeUp.wakeOne();
//            }
//            else
//            {
//                emit consolePutData("ModAnswerReceived answer wrong command\n", 1);
//                return false;
//            }
//            break;
//    }

    return true;
}
