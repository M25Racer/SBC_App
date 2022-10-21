#include <QCoreApplication>
#include <message_box.h>
//#include <main.h>
#include <QDebug>
#include "statistics.h"
#include <crc16.h>
//#include "mainwindow.h"
#include "global_vars.h"

//using namespace Ui;
//class MainWindow;

static uint8_t messege_box_buffer[1024];
//static uint32_t global_packet_number_mid2 = 255;
//static uint32_t global_packet_number_mid3 = 255;

static uint8_t command;
static uint8_t message_id;
static uint8_t device_id;
static uint32_t packet_number = 0;

//typedef struct
//{
//    uint32_t packet_adr;
//    uint8_t command;
//    uint8_t message_id;
//    uint8_t master_address;
//    uint8_t own_address;
//    uint16_t data_len;
//} message_header;

static message_header message;
static uint16_t tx_len = 0;
//static uint32_t time_buffer[8];
//static int16_t synchro_delta[8];
//static int a = 0;
//static bool synchronised = false;
//static bool mid_2_status_reg_update = false;
//static bool mid_3_status_reg_update = false;
//static uint32_t Mi2Lost = 20;
//static uint32_t Mi3Lost = 30;
//static uint32_t const version[2] = {220111,2};

uint8_t mod_status_reg;
//static uint8_t tool_status_reg[3];

//extern fifo_buffer_handle fifo_buffer;

// TODO placeholder vars, needs to be removed later
//static uint8_t temp_baudrate = 0;
//static uint8_t temp_att = 0;
//static uint8_t temp_adj = 0;
//static uint8_t temp_toolinfo[256];

//extern MainWindow *p_w;

//bool MessageBox::srp_packet_check(uint16_t len)
//{
//    // Check length
//    // len < minimum packet size ?
//    if(len < TX_HEADER_LENGTH + DATA_ADDRESS_LENGTH + CRC_LENGTH)
//    {
//        qDebug() << "Broken packet from PC to SRP: length error";
//        return false;
//    }

//    // Check crc
//    uint16_t crc16 = uint16_t(messege_box_buffer[len - 2] | (uint16_t(messege_box_buffer[len - 1]) << 8));
//    if(calc_crc16(messege_box_buffer + 1, len-1-CRC_LENGTH) != crc16)
//    {
//        qDebug() << "Broken packet from PC to SRP: crc error";
//        return false;
//    }

//    return true;
//}

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

//        case GET_TIME:
//            //messege_box_buffer[0] = MOD2_ADDR;
//            //post_tx_data(messege_box_buffer, tx_len, ami, line, get_global_baudrate());

////            protocol_time = get_time();
////            messege_box_buffer[11] = protocol_time.time_s;
////            messege_box_buffer[12] = protocol_time.time_s >> 8;
////            messege_box_buffer[13] = protocol_time.time_s >> 16;
////            messege_box_buffer[14] = protocol_time.time_s >> 24;
//            messege_box_buffer[15] = 0;
//            messege_box_buffer[16] = 0;
//            messege_box_buffer[17] = 0;
//            messege_box_buffer[18] = 0;
//            message.data_len = 8;
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case STATUS:
//            message.data_len = 1;
//            messege_box_buffer[11] = mod_status_reg;
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            //post_tx_data(messege_box_buffer, tx_len);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case CABLE_START:
//            mod_status_reg = ewl_work_status;
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case CABLE_PAUSE:
//            mod_status_reg = paused_status;
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case CABLE_STOP:
//            mod_status_reg = paused_status;
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case VERSIONS:
//            message.data_len = 8;
//            messege_box_buffer[11] = uint8_t(version[1]);
//            messege_box_buffer[12] = uint8_t(version[1] >> 8);
//            messege_box_buffer[13] = uint8_t(version[1] >> 16);
//            messege_box_buffer[14] = uint8_t(version[1] >> 24);
//            messege_box_buffer[15] = uint8_t(version[0]);
//            messege_box_buffer[16] = uint8_t(version[0] >>8);
//            messege_box_buffer[17] = uint8_t(version[0] >>16);
//            messege_box_buffer[18] = uint8_t(version[0] >>24);

//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case SET_BAUDRATE:
////            set_global_baudrate(messege_box_buffer[11]);
////            change_baudrate(messege_box_buffer[11]);
//            temp_baudrate = messege_box_buffer[11];
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case GET_BAUDRATE:
//            //messege_box_buffer[11] = get_global_baudrate();
//            messege_box_buffer[11] = temp_baudrate;

//            message.data_len = 1;
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

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

//        case SET_RX_PARAMETERS:
////            att_adj(messege_box_buffer[11]);
////            amp_adj(messege_box_buffer[12]);
//            temp_att = messege_box_buffer[11];
//            temp_adj = messege_box_buffer[12];
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case GET_RX_PARAMETERS:

////            messege_box_buffer[11] = get_att_adj();
////            messege_box_buffer[12] = get_amp_adj();
//            messege_box_buffer[11] = temp_att;
//            messege_box_buffer[12] = temp_adj;

//            message.data_len = 2;
//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case GET_TEMPRATURE:

// //           ext_temprature = read_i2c_temprature();
// //           int_temrature = read_adc();
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case GET_MEASURE_BUFFER:
////            protocol_time = get_time();
////            //ext_temprature = read_i2c_temprature();
////            int_temrature = read_adc();

////            messege_box_buffer[11] = protocol_time.time_s;
////            messege_box_buffer[12] = protocol_time.time_s >> 8;
////            messege_box_buffer[13] = protocol_time.time_s >> 16;
////            messege_box_buffer[14] = protocol_time.time_s >> 24;
//            messege_box_buffer[15] = 0;
//            messege_box_buffer[16] = 0;
//            messege_box_buffer[17] = 0;
//            messege_box_buffer[18] = 0;
////            messege_box_buffer[19] = protocol_time.time_ms;
////            messege_box_buffer[20] = protocol_time.time_ms >> 8;

////            messege_box_buffer[21] = ext_temprature;
////            messege_box_buffer[22] = ext_temprature >> 8;
//            messege_box_buffer[23] = 0;
//            messege_box_buffer[24] = 0;

////            messege_box_buffer[25] = int_temrature;
////            messege_box_buffer[26] = int_temrature >> 8;
////            messege_box_buffer[27] = int_temrature >> 16;
////            messege_box_buffer[28] = int_temrature >> 24;

//            messege_box_buffer[29] = 250;
//            messege_box_buffer[30] = 0;
//            messege_box_buffer[31] = 0;
//            messege_box_buffer[32] = 0;

//            messege_box_buffer[33] = 10;
//            messege_box_buffer[34] = 0;
//            messege_box_buffer[35] = 0;
//            messege_box_buffer[36] = 0;

//            message.data_len = 26;
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case SET_LINE_ERRORS:

////            statisics_reset();

//            break;

//        case SET_CUSTOM_PARAM:

//            suspend_time = messege_box_buffer[12]
//                        | (uint16_t) messege_box_buffer[13] << 8;
//            suspend_shift = messege_box_buffer[14]
//                        | (uint16_t) messege_box_buffer[15] << 8;

////            syncro_master_set(messege_box_buffer[11], suspend_time, suspend_shift);//TODO

//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            //message.data_len = 0;
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case GET_CUSTOM_PARAM:
////            suspend_time = syncro_master_status(2);
////            suspend_shift = syncro_master_status(3);

////            messege_box_buffer[11] = syncro_master_status(1);
////            messege_box_buffer[12] = suspend_time;
////            messege_box_buffer[13] = suspend_time >> 8;
////            messege_box_buffer[14] = suspend_shift;
////            messege_box_buffer[15] = suspend_shift >> 8;

//            message.data_len = 5;
//            tx_len = message_header_to_array(&message, messege_box_buffer);

//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case SYNCRO:

//            //syncro_master_reset();

//            break;

//        case SET_INFO:
////            HAL_FLASH_Unlock();
////            FLASH_Erase_Sector(FLASH_SECTOR_TOTAL - 2, FLASH_VOLTAGE_RANGE_3);
////            for(int i = 0; i < 256; i++){
////            HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 0x2ffff + i, messege_box_buffer[i + 11]);
////            }
////            HAL_FLASH_Lock();

//            for(int i = 0; i < 256; i++)
//                temp_toolinfo[i] = messege_box_buffer[i + 11];

//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

//        case GET_INFO:
////            for(int i = 0; i < 256; i++){
////            messege_box_buffer[i + 11] = (*(__IO uint32_t*) (FLASH_END - 0x2ffff + i));
////            }
//            for(int i = 0; i < 256; i++)
//                messege_box_buffer[i + 11] = temp_toolinfo[i];

//            message.data_len = 256;

//            tx_len = message_header_to_array(&message, messege_box_buffer);
//            emit postData(messege_box_buffer + 1, tx_len);
//            break;

        case AUTO_CFG_PREDISTORTION:
            emit commandCalculatePredistortionTablesStart();
            tx_len = message_header_to_array(&message, messege_box_buffer);
            emit postData(messege_box_buffer + 1, tx_len - 1);
            break;

        case GET_AUTO_CFG_STATUS:
            message.data_len = 1;
            mutex.lock();
            messege_box_buffer[11] = m_statusAutoCfgPredistortion;
            mutex.unlock();
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
                messege_box_buffer[11] = Get_AGC_State();
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

//uint16_t message_header_to_array_mid(const message_header* message, uint8_t* Buf)
//{
//    Buf[0] = message->master_address;
//    Buf[1] = message->own_address;
//    Buf[2] = message->command;
//    Buf[3] = message->message_id;
//    Buf[4] = 0x01; //TODO

//    uint16_t message_len = 0;
//    uint16_t crc_len = 0;
//    message_len = TX_HEADER_LENGTH + message->data_len;
//    Buf[5] = message->data_len >> 8;
//    Buf[6] = message->data_len;

//    crc_len = message_len;
//    uint16_t crc = calc_crc16_xmodem(Buf + 1, crc_len - 1);

//    Buf[message_len] = crc;
//    Buf[message_len + 1] = crc >> 8;
//    message_len += CRC_LENGTH;

//    return message_len;
//}

//void set_tool_status(tool_status status, uint8_t device_id)
//{
//    if (device_id == MID2_ADDR)
//    {
//        if (mid_3_status_reg_update)
//        {
//            //temp_status = get_tool_status(MID3_ADDR);
//            HAL_FLASH_Unlock();
//            //FLASH_Erase_Sector(FLASH_SECTOR_TOTAL - 1, FLASH_VOLTAGE_RANGE_3);
//            HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 128, status);
//            //HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 256,
//            // 		temp_status);
//            HAL_FLASH_Lock();
//            mid_2_status_reg_update = false;
//            mid_3_status_reg_update = false;
//        }
//        else
//        {
//            HAL_FLASH_Unlock();
//            FLASH_Erase_Sector(FLASH_SECTOR_TOTAL - 1, FLASH_VOLTAGE_RANGE_3);
//            HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 128, status);
//            HAL_FLASH_Lock();
//            mid_2_status_reg_update = true;
//        }
//        tool_status_reg[0] = status;
//    }
//    else if(device_id == MID3_ADDR)
//    {
//        if (mid_2_status_reg_update)
//        {
//            //temp_status = get_tool_status(MID2_ADDR);
//            HAL_FLASH_Unlock();
//            //FLASH_Erase_Sector(FLASH_SECTOR_TOTAL - 1, FLASH_VOLTAGE_RANGE_3);
//            //HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 128,
//            //		temp_status);
//            HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 256, status);

//            HAL_FLASH_Lock();
//            mid_3_status_reg_update = false;
//            mid_2_status_reg_update = false;
//        }
//        else
//        {
//            HAL_FLASH_Unlock();
//            FLASH_Erase_Sector(FLASH_SECTOR_TOTAL - 1, FLASH_VOLTAGE_RANGE_3);
//            HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, FLASH_END - 256, status);
//            HAL_FLASH_Lock();
//            mid_3_status_reg_update = true;
//        }
//        tool_status_reg[1] = status;
//    }
//    else if (device_id == MOD2_ADDR)
//    {
//        tool_status_reg[2] = status;
//    }
//}

//tool_status get_tool_status(uint8_t device_id)
//{
//    if (device_id == MID2_ADDR)
//    {
//        return (*(__IO uint32_t*) (FLASH_END - 128));
//    }

//    return (*(__IO uint32_t*) (FLASH_END - 256));
//}

//void send_ami_synchro()
//{
//    global_time current_time;

//    uint8_t synchro_buffer[32];
//    current_time = get_time();

//    synchro_buffer[11] = current_time.time_s;
//    synchro_buffer[12] = current_time.time_s >> 8;
//    synchro_buffer[13] = current_time.time_s >> 16;
//    synchro_buffer[14] = current_time.time_s >> 24;
//    synchro_buffer[15] = 0; //current_time.time_s >> 32;
//    synchro_buffer[16] = 0; //current_time.time_s >> 40;
//    synchro_buffer[17] = 0; //current_time.time_s >> 48;
//    synchro_buffer[18] = 0; //current_time.time_s >> 56;

//    message.master_address = MOD2_ADDR;
//    message.own_address = SRP2_ADDR;
//    message.data_len = 8;
//    message.command = SYNCRO;
//    message.packet_adr = 0;
//    tx_len = message_header_to_array(&message, synchro_buffer);

//    post_tx_data(synchro_buffer, tx_len, ami, line, get_global_baudrate());
//}

//void answer_messege(uint8_t* Buf, uint16_t len)
//{
//    memmove(messege_box_buffer + 8, Buf, len);

//    messege_box_buffer[7] = device_id;
//    message.data_len = len + 1;
//    tx_len = message_header_to_array_mid(&message, messege_box_buffer);

//    post_tx_data(messege_box_buffer, tx_len, ami, line, get_global_baudrate());
//}

void CMessageBox::setStatusAutoCfgPredistortion(uint8_t status)
{
    mutex.lock();
    m_statusAutoCfgPredistortion = status;
    mutex.unlock();
}
