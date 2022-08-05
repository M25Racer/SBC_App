#pragma once

#if !defined (_MSC_VER)
    #include "stdint.h"
#endif

#define TRANSPORT_FLAG_ONE_FRAGMENT       (0x0)
#define TRANSPORT_FLAG_FRAGMENTATION      (0x1)
#define TRANSPORT_FLAG_FRAGMENTATION_LAST (0x2)

#define COMMAND_TYPE_COMMAND              (1)
#define COMMAND_TYPE_ERROR                (2)
#define COMMAND_TYPE_REPEAT_REQUEST       (3) 

#define INDIGO_PROTOCOL_MAX_OFFSETS       (16)

#define BROADCAST_TOOL_ID                 (0x00)
#define BROADCAST_SERIAL_NUMBER           (0x00)

const uint8_t CMD_SCAN             = 0x0A;
const uint8_t CMD_CYCLO            = 0x5B;            
const uint8_t CMD_SCENARIO         = 0x6B; 
const uint8_t CMD_CUSTOM_PARAMS	   = 0xD9;

const uint8_t CMD_TIME             = 0xAA;
const uint8_t CMD_ACTION           = 0xAB;
const uint8_t CMD_GET_DATA         = 0xAC;

const uint8_t CMD_CLEAR_MEMORY     = 0xCB;
const uint8_t CMD_GET_STATUS       = 0x5C;
const uint8_t CMD_RAW_DATA         = 0x7A;
const uint8_t CMD_CALIBRATION      = 0x6F;

const uint8_t CMD_INFO		       = 0x3A;
const uint8_t CMD_PING             = 0xD8;
const uint8_t CMD_MONITOR          = 0x11;

const uint8_t NAND_PAGE_TEST       = 0xF1;	//TODO
const uint8_t NAND_SPARE_TEST      = 0xF2;	//TODO
const uint8_t READ_NAND_PAGE       = 0x10;	//TODO
const uint8_t FAST_READ_NAND_PAGE  = 0x19;	//TODO
const uint8_t WRITE_NAND_PAGE      = 0x80;	//TODO
const uint8_t READ_NAND_SPARE      = 0x20;	//TODO
const uint8_t WRITE_NAND_SPARE     = 0xA0;	//TODO

const uint8_t CMD_SWITCH_SPEED     = 0xDD;

const uint8_t ADAPTER_ADDRES_ADAPATER      = 0x5D;
const uint8_t ADAPTER_CMD_SWITCH_LOWSPEED  = 0x8C;
const uint8_t ADAPTER_CMD_SWITCH_HIGHSPEED = 0x3C;

const uint8_t SYNCHRO              = 0xF3;	//TODO

const uint8_t CMD_REBOOT            = 0x3F;

// Synchronization 'suspended time' ('busy'\'wait') answer
const uint8_t CMD_WAIT              = 0xD1;

/*
const uint8_t CMD_START            = 0x1C;
const uint8_t CMD_STOP             = 0x0C;
const uint8_t CMD_SCAN             = 0x0A;
const uint8_t CMD_SET_MODE         = 0x8C;
const uint8_t CMD_GET_MODE         = 0x7C;
const uint8_t CMD_GET_SYNCHRO_MODE = 0x7D;
const uint8_t CMD_SET_TIME         = 0x1A;
const uint8_t CMD_GET_TIME         = 0x2A;
const uint8_t CMD_GET_INFO         = 0x4A;
const uint8_t CMD_SET_SERIAL       = 0x3A;
const uint8_t CMD_SET_CYCLO        = 0x5B;
const uint8_t CMD_GET_CYCLO        = 0x6B;
const uint8_t CMD_GET_MEM          = 0x7B;
const uint8_t CMD_GET_MEM_ST       = 0xBC;
const uint8_t CMD_GET_FREE         = 0x9B;
const uint8_t CMD_GET_SIZE         = 0x9A;
const uint8_t CMD_GET_MEM_INFO     = 0xBA;
const uint8_t CMD_GET_CALIB        = 0x6F;
const uint8_t CMD_SET_CALIB        = 0x5F;
const uint8_t CMD_SET_DELAY        = 0x4E;
const uint8_t CMD_GET_DELAY        = 0x4F;
const uint8_t CMD_SET_MASTER       = 0x4C;
const uint8_t CMD_GET_MASTER       = 0x4D;
const uint8_t CMD_MEAS_GET         = 0xA8;
const uint8_t CMD_MEAS_SCR         = 0xCC;
const uint8_t CMD_GET_DATA         = 0xAC;
const uint8_t CMD_CLEAR            = 0xAA;
const uint8_t CMD_CLEAR_FULL       = 0xAC;
const uint8_t CMD_CLEAR_PSEUDO     = 0xAA;
const uint8_t CMD_OPEN_WR          = 0xB8;
const uint8_t CMD_CLOSE_WR         = 0xB9;

const uint8_t CMD_BASE_SCAN        = 0xC0;
const uint8_t CMD_EWL_START        = 0xC1;
const uint8_t CMD_EWL_PAUSE        = 0xC2;
const uint8_t CMD_EWL_STOP         = 0xC3;
const uint8_t CMD_EWL_GETSIZE      = 0xC4;
const uint8_t CMD_EWL_GETDATA      = 0xC5;
const uint8_t CMD_EWL_SET_PARAMETERS = 0xC6;
const uint8_t CMD_EWL_GET_PARAMETERS = 0xC7;
const uint8_t CMD_EWL_GET_STATUS   = 0xC8;

const uint8_t CMD_EWL_MODEM_ADDR   = 0x54;
*/


/* ERRORS */
const uint8_t ERROR_UNKNOWN_CMD   = 0xE1; ///< Команда не распознана
const uint8_t ERROR_BAD_DATA      = 0xE2; ///< Поле данных не удалось распарсить
const uint8_t ERROR_HARDWARE_AMI  = 0xE3; ///< Ошибка работы с AMI кодеком

/* Device specific commands (can overlap with each other) */
const uint8_t CMD_CHORUS_GEN       = 0xD0;
