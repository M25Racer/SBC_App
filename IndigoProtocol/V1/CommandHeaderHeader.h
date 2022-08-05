#pragma once

/*!
* Базовый заголовок
*/
#pragma pack(push, 1)
struct CmdCommonBase
{
    uint8_t  version;
    uint8_t  type;
};
#pragma pack(pop)


/*!
* Базовый заголовок типа команды
*/
struct CmdCommonHeaderV1 : CmdCommonBase 
{
};

/*!
* Базовый заголовок типа команды повторение запрос
*/
#pragma pack(push, 1)
struct CmdRepeatHeaderV1 : CmdCommonBase
{
    uint8_t count; // колво смещений
};
#pragma pack(pop)

/*!
* Константы для команды повторение запроса 
*/
typedef enum : uint32_t
{
    CmdRepeatEndData = 0xffffffffUL, // константа означает конец данных
} EnumCmdRepeat;

/*!
* Информация о смещениях 
*/
#pragma pack(push, 1)
struct CmdRepeatOffset
{
    uint32_t offset;
    uint32_t length;
};
#pragma pack(pop)

/*!
* Заголовок ошибки
*/
#pragma pack(push, 1)
struct CmdTypeErrorHeaderV1 : CmdCommonBase
{
    uint16_t  frame_num;
    uint8_t   cmd_id;
    uint16_t  error_id;
    char      error_str[32];
};
#pragma pack(pop)
