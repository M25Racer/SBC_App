#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include <QCoreApplication>

#pragma pack(push, 1)

typedef struct
{
    uint32_t packet_adr;
    uint8_t command;
    uint8_t message_id;
    uint8_t master_address;
    uint8_t own_address;
    uint16_t data_len;
} message_header;

typedef enum {
    ready_status = 1,
    work_status = 2,
    idle_status = 8,
    ewl_work_status = 12,
    paused_status = 14
}tool_status;

#pragma pack(pop)

class MessageBox : public QObject
{
public:
    Q_OBJECT

signals:
    void postData(const uint8_t *p_data, const int length);
    void postDataToStm32H7(const uint8_t *p_data, const int length);

public:
    bool message_box_srp(uint8_t* Buf, uint16_t len, uint8_t master_address, uint8_t own_address);
    uint16_t message_header_to_array(const message_header* message, uint8_t* Buf);

    const uint8_t SRP_ADDR  = 0x53;
    const uint8_t MOD_ADDR 	= 0x54;
    const uint8_t MASTER_ADDR = 0x4D;

    const uint8_t PT_ADDR    = 0x44;
    const uint8_t SILS_ADDR  = 0x45;
    const uint8_t SFBS_ADDR  = 0x46;
    const uint8_t NAV_ADDR   = 0x47;
    const uint8_t ReCap_ADDR = 0x52;
    const uint8_t XYC_ADDR   = 0x43;
    const uint8_t HEX_ADDR   = 0x48;
    const uint8_t Ind1h_ADDR = 0x21;

    const uint32_t RX_HEADER_LENGTH = 6;
    const uint32_t TX_HEADER_LENGTH = 7;
    const uint32_t DATA_ADDRESS_LENGTH = 4;
    const uint32_t CRC_LENGTH = 2;
    const uint32_t DATA_LENGTH = 256;
    const uint32_t FRAME_LENGTH = (TX_HEADER_LENGTH + DATA_ADDRESS_LENGTH + DATA_LENGTH + CRC_LENGTH);

//    const uint8_t INFO_ADDRESS          = 0x00;

protected:

private:
    static const uint8_t GOOD_QUICK_ANSWER     = (0x1D);
    static const uint8_t BAD_QUICK_ANSWER      = (0x0D);

    static const uint8_t SET_ATT               = 0xA1;    //	Установка аттенюатора 0 - 7
    static const uint8_t SET_AMP               = 0xA2;    //	Установка усилениея 0 - 255
    static const uint8_t SET_DETECT            = 0xA3;    //	Установка ширины окна детектора 0 - 15

    static const uint8_t SCAN                  = 0x0A;    //	сканирование
    static const uint8_t SET_TIME              = 0x1A;	//	установка времени
    static const uint8_t GET_TIME              = 0x2A;  	//  запрос времени &  текущих данных
    static const uint8_t SET_INFO              = 0x3A;	//	принять калибровки
    static const uint8_t GET_INFO              = 0x4A;	//	запрос калибровок
    static const uint8_t SET_CYCLOGRAM         = 0x5A;	//	принять циклограмму
    static const uint8_t GET_CYCLOGRAM         = 0x6A;	//	запрос циклограммы
    static const uint8_t GET_DATA              = 0x7A;	//	запрос данных
    static const uint8_t GET_SIZE              = 0x9A;	//	запрос кол-ва данных в памяти
    static const uint8_t GET_POSITION          = 0x8A;	//	запрос текущей позиции
    static const uint8_t STOP                  = 0x0C;	//	старт
    static const uint8_t START                 = 0x1C;	//	стоп
    static const uint8_t STATUS                = 0x5C;	//	статус прибора
    // ///////////команды синхронизации
    static const uint8_t STATUSS               = 0x7D;	//	статус параметров синхронизации
    static const uint8_t SYNCRO                = 0x2D;	// Синхронизация цикла приборов
    static const uint8_t SETDELAY              = 0x4E;	// Установка задержки запуска измерения от синхронизации
    static const uint8_t CLRMASTER             = 0x3C;	// Отмена режима мастера синхронизации
    static const uint8_t SETMASTER         	= 0x4C;	// Запуск мастера синхронизации
    static const uint8_t BLACKOUT              = 0x1B;	// Сообщения о блекаутах
    static const uint8_t VERSIONS              = 0x2B;	// Информация о версиях
    static const uint8_t SET_CALIBRATION   	= 0x3B;	// Установить калибровочную таблицу
    static const uint8_t GET_CALIBRATION   	= 0x4B;	// Получить калибровочную таблицу
    // //////////////// cable commands
    static const uint8_t CABLE_START       	= 0xC1;
    static const uint8_t CABLE_PAUSE       	= 0xC2;
    static const uint8_t CABLE_STOP        	= 0xC3;
    static const uint8_t CABLE_SET_EWL         = 0xC6;
    static const uint8_t CABLE_GET_EWL         = 0xC7;
    static const uint8_t CABLE_GET_SIZE    	= 0xC4;
    static const uint8_t CABLE_GET_DATA    	= 0xC5;
    static const uint8_t CABLE_ERASE       	= 0xCB;
    static const uint8_t CABLE_STATUS          = 0xC8;
    // /////////////// mod commands
    static const uint8_t SET_BAUDRATE      	= 0xD0;
    static const uint8_t GET_BAUDRATE          = 0xD1;
    static const uint8_t GET_LINE_ERRORS       = 0xD2;
    static const uint8_t SET_RX_PARAMETERS     = 0xD3;
    static const uint8_t GET_RX_PARAMETERS     = 0xD4;
    static const uint8_t GET_TEMPRATURE    	= 0xD5;
    static const uint8_t GET_MEASURE_BUFFER	= 0xD6;
    static const uint8_t SET_LINE_ERRORS		= 0xD7;
    static const uint8_t PING                  = 0xD8;
    static const uint8_t SET_CUSTOM_PARAM      = 0xD9;
    static const uint8_t GET_CUSTOM_PARAM      = 0xDA;
};


//void message_box_srp(uint8_t* Buf, uint16_t len, uint8_t master_address, uint8_t own_address);

//uint16_t message_header_to_array(const message_header* message, uint8_t* Buf);

#endif // MESSAGE_BOX_H
