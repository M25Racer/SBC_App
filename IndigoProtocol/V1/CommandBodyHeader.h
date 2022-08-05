#pragma once

#include "TransportProtocolHeader.h"

/*!
* Заголовок команды
*/
#pragma pack(push, 1)
struct CmdTypeCommandHeader
{
    uint8_t  version;
    uint8_t  cmd_id;
};
#pragma pack(pop)

/*
* команды Down формирует - DLL
* команды Up формирует  - прошивка
*/

/*
* Команда SCAN
*/
#pragma pack(push, 1)
struct CmdScanDownV1
{
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdScanUpV1
{
    ToolAddress address;
};
#pragma pack(pop)

/*
\breif Команда CMD_SCENARIO

Сценарии работы

Получение списка сценариев

Запрос
1 operand (ScenarioOperandGet)

Ответ
1                                  operand (ScenarioOperandGet)              
1                                  count                       колво сценариев
count * sizeof(CmdScenarioItemV1)  массив CmdScenarioItemV1



Сохранение списка сценариев

Запрос
1                                  operand (ScenarioOperandSet)
1                                  count колво сценариев
count * sizeof(CmdScenarioItemV1)  массив CmdScenarioItemV1


Ответ
1     operand (ScenarioOperandSet)
*/

typedef enum : uint8_t
{
    ScenarioOperandGet = 1,
    ScenarioOperandSet = 2,
} EnumScenarioOperand;

/*!
count - количество элементов
*/
typedef uint8_t CmdScenarioCountV1;


/*!
элемент сценария
*/
#pragma pack(push, 1)
struct CmdScenarioItemV1
{
    uint8_t       id;            ///< ID сценария 
    uint8_t       sensor_id;     ///< id сенсора для которого предназначен сценарий 
    char          name[8];       ///< имя сценария 
	uint16_t      period;        ///< период опроса сенсора
	uint16_t      length;        ///< длина сырых данных сценария 
};
#pragma pack(pop)

/*!
укатель на начало сырых данных для сценария
*/
typedef uint8_t* CmdScenarioData;

/*
 * \brief Команда CUSTOM_PARAMS
 */
typedef enum : uint8_t
{
    CustomParamsOperandGet = 1,
    CustomParamsOperandSet = 2,
} EnumCustomParamsOperand;

#pragma pack(push, 1)
struct CmdCustomParamsItemV1
{
   bool SynchMasterFlag;
};
#pragma pack(pop)

/*
\brief Команда CMD_CYCLO

Получение списка циклограмм

Запрос
operand = CycloOperandGet

Ответ
operand = ScenarioOperandGet
count = колво строчек циклограмм
массив циклограмм
    work time общее время работы циклограммы
    count колво CmdCycloItemV1 которые будут работать в текущей строчке циклограммы
    массив CmdCycloItemV1

CmdCycloParamsV1




Сохранение списка циклограмм

Запрос
operand = ScenarioOperandSet
count = колво строчек циклограмм
массив циклограмм
    work time общее время работы циклограммы
    count колво CmdCycloItemV1 которые будут работать в текущей строчке циклограммы
    массив CmdCycloItemV1

CmdCycloParamsV1

Ответ
operand = CycloOperandSet
*/

typedef enum : uint8_t
{
    CycloOperandGet = 1,
    CycloOperandSet = 2,
    CycloOperandEwlSet = 3,
    CycloOperandEwlGet = 4
} EnumCycloOperand;

/*!
count - количество элементов
*/
typedef uint8_t CmdCycloCountV1;

/*!
время работы циклограммы в ms
*/
typedef uint32_t CmdCycloItemTimeMsV1;
/*!
колво сенсоров которые будут работать в текущей циклограмме
*/
typedef uint8_t  CmdCycloItemCountV1;

/*!
элемент циклограммы
*/
#pragma pack(push, 1)
struct CmdCycloItemV1
{
    uint8_t    sensor_id;     ///< сенсор ID который будет включен в текущей циклограмме
    uint8_t    scenario_id;   ///< ID сценария
    uint8_t    sensor_enable; ///< работает или не работает сенсор в данный интервал
};
#pragma pack(pop)

/*!
режим и параметры в которых работает циклограмма
*/
#pragma pack(push, 1)
struct CmdCycloParamsV1
{
    uint8_t    mode;    ///< Work/Pause 
    uint8_t    marker;  ///< Cyclo/Ewl 
};
#pragma pack(pop)


/*!
\brief Kол-во элементов макси для частичной передачи по EWL

при отстуствие масок count = 0
*/
typedef uint8_t  CmdEWLMaskCountV1;


/*!
\brief Макса для частичной передачи по EWL

стурктура для CycloOperandEWL
*/
#pragma pack(push, 1)
struct CmdEWLMaskV1
{
    uint16_t offset;
    uint16_t length;
};
#pragma pack(pop)


/*!
 \brief Структура команды запроса страницы NAND
 */
#pragma pack(push, 1)
struct CmdReadNandPageDownV1
{
    uint32_t   page_addr;///< номер страницы
    uint16_t   page_num;///< количество запрашиваемых страниц
};
#pragma pack(pop)

/*!
 \brief Структура команды ответа страницы NAND
 */
#pragma pack(push, 1)
struct CmdReadNandPageUpV1
{
    uint32_t   page_addr;///< номер страницы
    uint8_t*   p_page_data;///< указатель на данные
};
#pragma pack(pop)

/*
Команда CMD_ACTION
*/

typedef enum : uint8_t
{
    ActionModeMemory = 1,
    ActionModeEWL,
} EnumActionMode;

typedef enum: uint8_t
{
    ActionOperandStart = 1,
    ActionOperandStop,
    ActionOperandPause,
} EnumActionOperand;

#pragma pack(push, 1)
struct CmdActionV1
{
    uint8_t mode;
    uint8_t operand;
};
#pragma pack(pop)

/*!
Команда CMD_TIME
*/
typedef enum : uint8_t
{
    TimeOperandSet = 1,
    TimeOperandGet,
} EnumTimeOperand;

#pragma pack(push, 1)
struct CmdTimeOperandV1
{
    uint8_t operand;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdTimeValueV1
{
    uint64_t sec;
};
#pragma pack(pop)


/*
Передача данных в DLL
*/

/*
Флаги для маркировки чанков
*/
typedef enum : uint8_t
{
	DataFlagsChunkSingle   = 0x0,  /* единственный чанк в семпле */
    DataFlagsChunkNotFirst = 0x1,  /* не первый чанк в семпле    */
    DataFlagsChunkNotLast = 0x2,   /* не последний чанк в семпле */
    DataFlagsChunkMiddle = DataFlagsChunkNotFirst | DataFlagsChunkNotLast,
    /*
    Пример, семпл состоит из трех чанков
    1 чанк (начало) содержит DataFlagsChunkNotLast
    2 чанк (средний) содержит DataFlagsChunkNotFirst | DataFlagsChunkNotLast
    3 чанк (конец) содержит DataFlagsNotFirst
    */
} EnumDataFlags;

#pragma pack(push, 1)
struct IndigoSampleTime
{
    uint64_t   sec;
    uint16_t   ms;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct IndigoMemorySampleV1
{
    uint32_t sample_id;
    uint8_t  sensor_id;
    IndigoSampleTime time;
    uint8_t  cyclo_num;
    uint8_t  scen_link;
    uint16_t Dlen;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct IndigoBaseMemorySampleChunk
{
	uint32_t sample_id;
	uint8_t  sensor_id;
    uint8_t  flags;
	uint16_t len;
};
#pragma pack(pop)

/*
Заголовок IndigoAdditionalMemorySampleChunk содержится ТОЛЬКО в первом чанке семпла, 
(когда поле flags имеет значение DataFlagsChunkNotLast)
*/
#pragma pack(push, 1)
struct IndigoAdditionalMemorySampleChunk
{
	IndigoSampleTime time;
	uint8_t  cyclo_num;
	uint8_t  scen_link;
};
#pragma pack(pop)

/*!
\brief CMD_GET_DATA


Размер файла
Все измерение
или 
колво данных по строкам циклограммы
номер циклограммы, id сенсора

Настройка получения данных
колво данных
номер циклограммы, номер сенсора, маска получения

Получение данных
номер циклограммы, номер сенсора, режим смещения, оффсет, размер скачиваемых данных
*/

typedef enum : uint8_t
{
    EnumGetDataOperandSize = 1, 
    EnumGetDataOperandSettings = 2,
    EnumGetDataOperandData = 3
} EnumCmdGetDataOperandV1;

typedef enum : uint8_t
{
    EnumGetDataDirectionBegin = 1, 
    EnumGetDataDirectionLast = 2,
} EnumCmdGetDataDirectionV1;

#pragma pack(push, 1)
struct CmdGetDataCursorV1
{
    uint64_t  last_id;
    uint32_t  count;
};
#pragma pack(pop)
const uint32_t CmdGetDataFirstID = 0;

#pragma pack(push, 1)
struct CmdGetDataDownV1
{
    EnumCmdGetDataDirectionV1 mode;
    CmdGetDataCursorV1 cursor;
};
#pragma pack(pop)
typedef CmdGetDataCursorV1 CmdGetDataUpV1;
typedef uint8_t* CmdGetDataSamplesV1;



/*!
CMD_GET_STATUS
*/
#pragma pack(push, 1)
struct CmdGetStatusV1
{
    uint8_t status;
};
#pragma pack(pop)


/*!
CMD_GET_RAW_DATA
*/
typedef enum : uint8_t
{
    EnumGetRawDataOperandSize = 1,
    EnumGetRawDataOperandData = 2,
    EnumGetRawDataOperandInfo = 3

} EnumCmdGetRawDataOperandV1;

#pragma pack(push, 1)
struct CmdGetRawDataSize
{
	uint64_t nand_size;       ///< Размер флешки в байтах
	uint16_t page_data_size;  ///< Размер страницы данных в байтах
	uint16_t page_spare_size; ///< Размер страницы резервных данных в байтах
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdGetRawDataV1
{
    uint64_t offset;
    uint64_t length;
};
#pragma pack(pop)

typedef uint8_t *CmdGetRawDataPtr;

#pragma pack(push, 1)
struct CmdGetRawDataInfo
{
    uint64_t filled_with_data_size; ///< Размер используемой памяти в байтах за текущее исследование
    uint64_t all_nand_size;         ///< Размер всей флешки в байтах
    uint16_t page_data_size;        ///< Размер страницы данных в байтах
    uint16_t page_spare_size;       ///< Размер страницы резервных данных в байтах
    uint64_t nand_id;               ///< ID флешки 
};
#pragma pack(pop)


#pragma pack(push, 1)
struct CmdCalibrationV1
{
    uint8_t       version;                 ///< версия калибровки
    uint64_t      calibration_datetime_s;  ///< dateTime калибровки
    uint16_t      length;                  ///< длина сырых данных калибровки
};
#pragma pack(pop)

/*!
укатель на начало сырых данных для калибровки
*/
typedef uint8_t* CmdCalibrationData;

typedef enum: uint8_t
{
    CalibrationOperandGet = 1,
    CalibrationOperandSet = 2,
} EnumCalibrationOperand;

typedef enum : uint8_t
{
    EnumInfoOperandSerialGet = 1,
    EnumInfoOperandSerialSet = 2,
	EnumInfoBlackOutGet      = 3,
	EnumInfoMemoryGet        = 4,
	EnumInfoLifeGet          = 5,
	EnumInfoMemoryUtil       = 6,
} EnumInfoOperandV1;

#pragma pack(push, 1)
struct CmdInfoSerialGetV1
{
	uint16_t      serial;             ///< версия серийного номера 
    uint32_t      firmwareDate;       ///< дата прошивки
    uint32_t      firmware;           ///< версия прошивка 
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdInfoSerialSetV1
{
	uint16_t      serial;             ///< версия серийного номера 
};
#pragma pack(pop)


typedef uint8_t CmdBlackOutCountV1;
#pragma pack(push, 1)
struct CmdInfoBlackOutV1
{
	uint64_t      time_seconds;
	uint16_t	  time_miliseconds;
};
#pragma pack(pop)


#pragma pack(push, 1)
struct CmdInfoMemoryV1
{
	uint64_t      capacity;
	uint32_t	  badBlocks;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdInfoLifeV1
{
	uint32_t      working_time_seconds; 	///< время работы приборы
	uint32_t      working_time_ht_seconds;  ///< время работы приборы на высокой температуре
	uint32_t	  max_temperature; 			///< максимальная температура в каторой прибор работал
	uint32_t	  min_temperature; 			///< минимальная  температура в каторой прибор работал
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdInfoMemoryUtil
{
	uint8_t percents; ///< обьем занятой памяти в процентах
};
#pragma pack(pop)


typedef enum : uint8_t
{
    EnumClearOperandPerform = 1, ///< выполнить стирание
    EnumClearOperandStatus = 2  ///< вернуть статус стирания
} ClearOperandV1;

#pragma pack(push, 1)
struct CmdClearTimeV1
{
    uint32_t      time_miliseconds;     ///< время необходимое для стирания памяти прибора 
};
#pragma pack(pop)

typedef enum : uint8_t
{
    EnumClearOk = 1,
    EnumClearFail = 2
} EnumClearStatusV1;

#pragma pack(push, 1)
struct CmdClearStatusV1
{
    EnumClearStatusV1  status;      ///< статус стирания
    uint8_t            error_code;  ///< код ошибки
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdPingV1
{
    uint16_t length;
};
#pragma pack(pop)

////////////////////////////
/// CMD_SWITCH_SPEED
////////////////////////////

typedef enum : uint8_t
{
    SwitchSpeedOperandLow = 0,
    SwitchSpeedOperandHigh = 1,
} EnumSwitchSpeedOperand;



typedef enum : uint8_t
{
    EnumMonitorPerform = 1,
    EnumMonitorGetData = 2
} EnumMonitorV1;

#pragma pack(push, 1)
struct CmdMonitorWaitV1
{
    uint32_t time_miliseconds;     ///< время ожидания разового замера в режиме монитор
};
#pragma pack(pop)

////////////////////////////
/// CMD_CHORUS_GEN
////////////////////////////

typedef enum : uint8_t
{
	EnumChorusGeneratorOperandSet = 0,
} EnumChorusGeneratorOperand;

#pragma pack(push, 1)
struct CmdChorusGeneratorV1
{
	uint16_t      frequency_hz;    ///< частота сигнала в герцах (не менее 500 или 1000), при 0 – сигнал выключен
};
#pragma pack(pop)

////////////////////////////
/// CMD_REBOOT
////////////////////////////

#pragma pack(push, 1)
typedef enum : uint8_t
{
    RebootOperandPerform = 1,
} EnumRebootOperand;
#pragma pack(pop)

/*!
 Задержка перед reboot, мс
 */
typedef uint16_t CmdRebootTimeDelayMS;

#pragma pack(push, 1)
struct CmdWait
{
    uint16_t ms;
};
#pragma pack(pop)
