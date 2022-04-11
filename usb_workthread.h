#ifndef USB_WORKTHREAD_H
#define USB_WORKTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <libusb-1.0/libusb.h>
#include <ringbuffer.h>
#include <usb_global.h>

class UsbWorkThread : public QThread
{
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);
    void consoleAdcFile(const quint8 *adc_data, quint32 size);
    void postTxDataToSerialPort(const uint8_t *p_data, int len);
    void usbInitTimeoutStart(int timeout_ms);

public slots:
    void sendHsCommand(uint8_t Command, uint32_t Length, uint8_t *p_Data);

public:
    explicit UsbWorkThread(QObject *parent = nullptr);
    ~UsbWorkThread();

    void USB_ThreadStart();
    void USB_Init();
    void USB_Deinit();
    void USB_StopTransmit();

    // Init 'states'
    typedef enum {
        INIT = 0, OPEN_DEV, DEV_TAKE_CONTROL, CLAIM_INTERFACE, TRANSMIT_RECEIVE_INIT, INIT_END
    } UsbInitState;

    UsbInitState InitState = UsbInitState::INIT;

    uint8_t UserTxBuffer[65536];

    bool start_transmit = false;
    bool start_receive = false;
    bool start_receive_multiple = false;

    int UserTxBuffer_len = 0;

    bool usb_receiver_drop = false;     // Drop any received usb data when this flag is active

    const int timeoutUsbInit_ms = 1000;

    uint32_t stm32_ready_data_size = 0;

private:
    void run() override;

    bool usbInitialization();
    void USB_Reconnect();
    void USB_ReceiveTransmitInit();
    void USB_StartReceiveSingle();
    void USB_StartTransmit();
    void USB_StopReceiveSingle();
    void USB_StopReceiveMultiple();
    void parseHsData();

    // Hack to use libusb's 'C'-callback functions in 'C++' project
    // Use 'user_data' to pass pointer to 'this'
    static void LIBUSB_CALL static_tx_callback(struct libusb_transfer *transfer)
    {
        return reinterpret_cast<UsbWorkThread*>(transfer->user_data)->tx_callback(transfer);
    }
    static void LIBUSB_CALL static_rx_callback(struct libusb_transfer *transfer)
    {
        return reinterpret_cast<UsbWorkThread*>(transfer->user_data)->rx_callback(transfer);
    }
    static int LIBUSB_CALL static_hotplug_callback(libusb_context *ctx, libusb_device *device, libusb_hotplug_event event, void *user_data)
    {
        return reinterpret_cast<UsbWorkThread*>(user_data)->hotplug_callback(ctx, device, event, user_data);
    }

    void LIBUSB_CALL tx_callback(struct libusb_transfer *transfer);
    void LIBUSB_CALL rx_callback(struct libusb_transfer *transfer);
    int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *device, libusb_hotplug_event event, void *user_data);


    libusb_context *context = nullptr;
    libusb_device_handle *handle = nullptr;

    static const uint32_t n_RxTransfers = 4;
    struct libusb_transfer *t_rx = nullptr;
    struct libusb_transfer *t_tx = nullptr;
    struct libusb_transfer *t_rx_multiple[n_RxTransfers];

    bool m_quit = false;
    bool receive_in_progress = false;
    bool transmit_in_progress = false;

    int tx_complete_flag = 0;
    //int rx_complete_flag = 0;

    QElapsedTimer rx_timeout_timer;
    QElapsedTimer console_spam_timer;

    const qint64 rx_timeout_ms = 50;//10;            // usb rx timeout between transfers of one big packet, ms

    libusb_hotplug_callback_handle h_hotplug;   // libusb callback handle for hotplug event

    struct asyncParams
    {
        // Указатель на большой буфер, в который
        // мы принимаем массив. Он может быть размером
        // в сотни мегабайт
        uint8_t* pData;

        // Каждая транзакция принимает маленький кусочек.
        // Например, 64 килобайта. В этой переменной лежит
        // текущее смещение. Поставили транзакцию в очередь -
        // сдвинулись в буфере. Короче, это указатель
        // запрошенной части буфера.
        uint32_t dataOffset;

        // Размер буфера в байтах. Полезен, чтобы знать,
        // когда следует прекращать работу. Если смещение
        // дошло до этой величины - функция обратного вызова
        // перестанет создавать новые запросы на передачу
        int dataSizeInBytes;

        // Размер одной передачи
        int oneTransferLen;

        // Используется для отображения заполненности буфера,
        // увеличивается по факту прихода новой порции данных.
        // То есть, это указатель фактически заполненного буфера
        int actualReceived;

        // Is receive multitransfer active
        bool multitransferInProgress;

        // Current (awaited) packet length from header
        uint32_t packet_length;

    } m_asyncParams;
};

#endif // USB_WORKTHREAD_H
