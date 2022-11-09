#include <QCoreApplication>
#include <message_box.h>
#include <QDebug>
#include "statistics.h"
#include <crc16.h>
#include "atomic_vars.h"

static uint8_t messege_box_buffer[1024];

static uint8_t command;
static uint8_t message_id;
static uint8_t device_id;
static uint32_t packet_number = 0;

static message_header message;
static uint16_t tx_len = 0;

bool CMessageBox::message_box_srp(uint8_t* Buf, uint16_t len, uint8_t master_address, uint8_t own_address)
{
    if(len > sizeof(messege_box_buffer))
        len = sizeof(messege_box_buffer);

    memmove(messege_box_buffer, Buf, len);

    command = messege_box_buffer[2];
    message_id = messege_box_buffer[3];
    device_id = messege_box_buffer[7];
    //uint16_t ext_temprature = 0;
    //uint32_t int_temrature = 0;

//    data_quality quality;
//    global_time protocol_time;

//    uint16_t suspend_time = 0;
//    uint16_t suspend_shift = 0;

    tx_len = 0;
    packet_number = 0;
    //uint32_t current_tick = HAL_GetTick();
    message.command = command;
    message.own_address = own_address;
    message.master_address = master_address;
    message.message_id = message_id;
    message.data_len = message.packet_adr = 0;

    packet_number = messege_box_buffer[7]
                | uint32_t(messege_box_buffer[8]) << 8
                | uint32_t(messege_box_buffer[9]) << 16
                | uint32_t(messege_box_buffer[10]) << 24;

    switch (command)
    {
        case SCAN:
            tx_len = message_header_to_array(&message, messege_box_buffer);
            emit postData(messege_box_buffer + 1, tx_len - 1);
            break;

        case SET_TIME:
            // TODO save time for SBC needs and retransmit it to STM32H7
//            //messege_box_buffer[0] = MOD2_ADDR;
//            //post_tx_data(messege_box_buffer, tx_len, ami, line, get_global_baudrate());

////            protocol_time.time_s = messege_box_buffer[11]
////            | (uint32_t) messege_box_buffer[12] << 8
////            | (uint32_t) messege_box_buffer[13] << 16
////            | (uint32_t) messege_box_buffer[14] << 24;
////            set_time(protocol_time);
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);

            // Retransmit it to STM32H7 board
            emit postDataToStm32H7(messege_box_buffer, len);

            // Data was not answered by message box
            return false;
            break;

        case GET_LINE_ERRORS:
            // TODO if SBC is working in QAM mode, answer to this command
            // Else do not answer, retransmit it to STM32H7
//            quality = get_data_statistics();
//            messege_box_buffer[11] = uint8_t(quality.good_crc);
//            messege_box_buffer[12] = quality.good_crc >> 8;
//            messege_box_buffer[13] = uint8_t(quality.good_synhro);
//            messege_box_buffer[14] = quality.good_synhro >> 8;

//            message.data_len = 4;
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);

            // Retransmit it to STM32H7 board
            emit postDataToStm32H7(messege_box_buffer, len);

            // Data was not answered by message box
            return false;
            break;

        case AUTO_CFG_PREDISTORTION:
            emit commandCalculatePredistortionTablesStart();
            tx_len = message_header_to_array(&message, messege_box_buffer);
            emit postData(messege_box_buffer + 1, tx_len - 1);
            break;

        case GET_AUTO_CFG_STATUS:
            message.data_len = 1;
            messege_box_buffer[11] = m_statusAutoCfgPredistortion;
            tx_len = message_header_to_array(&message, messege_box_buffer);
            emit postData(messege_box_buffer + 1, tx_len - 1);
            break;

        case AGC_CONFIGURATION:
        {
            EnumAgcConfigCommand type = (EnumAgcConfigCommand)messege_box_buffer[11];
            if(type == EnumAgcConfigCommand::AGC_Start)
            {
                emit commandAgcStart();
            }
            else if(type == EnumAgcConfigCommand::AGC_GetStatus)
            {
                message.data_len = 1;
                messege_box_buffer[11] = AgcStateGlobal;
                tx_len = message_header_to_array(&message, messege_box_buffer);
                emit postData(messege_box_buffer + 1, tx_len - 1);
                break;
            }
            else
            {
                // Error, wrong command
                return false;
            }
            tx_len = message_header_to_array(&message, messege_box_buffer);
            emit postData(messege_box_buffer + 1, tx_len - 1);
        }
            break;

        case GET_SBC_STATISTICS:
        {
            uint8_t statistics = m_rxHighSpeedStatistics;
            uint8_t statistics2 = rs_calculate_statistics();
            uint16_t statistics3 = m_ReedSolomonCorrections;

            messege_box_buffer[11] = uint8_t(statistics);
            messege_box_buffer[12] = uint8_t(statistics2);

            messege_box_buffer[13] = uint8_t(statistics3);
            messege_box_buffer[14] = statistics3 >> 8;

            message.data_len = 4;
            tx_len = message_header_to_array(&message, messege_box_buffer);
            emit postData(messege_box_buffer + 1, tx_len - 1);
        }
            break;

        default:
            // Unknown command for SBC, retransmit it to STM32H7 board
            emit postDataToStm32H7(messege_box_buffer, len);

            // Data was not answered by message box
            return false;
            break;
    }

    // Data was parsed and answered by message box
    return true;
}

uint16_t CMessageBox::message_header_to_array(const message_header* message, uint8_t* Buf)
{
    Buf[0] = message->master_address;
    Buf[1] = message->own_address;
    Buf[2] = message->command;
    Buf[3] = message->message_id;
    Buf[4] = 0x01;

    uint16_t message_len = 0;

    message_len = TX_HEADER_LENGTH + DATA_ADDRESS_LENGTH + message->data_len;
    Buf[5] = uint8_t(DATA_ADDRESS_LENGTH + message->data_len);
    Buf[6] = (DATA_ADDRESS_LENGTH + message->data_len) >> 8;
    Buf[7] = message->packet_adr;
    Buf[8] = message->packet_adr >> 8;
    Buf[9] = message->packet_adr >> 16;
    Buf[10] = message->packet_adr >> 24;

    uint16_t crc = calc_crc16(Buf + 1, message_len - 1);

    Buf[message_len] = uint8_t(crc);
    Buf[message_len + 1] = crc >> 8;
    message_len += CRC_LENGTH;

    return message_len;
}
