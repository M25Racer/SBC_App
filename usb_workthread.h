#ifndef USB_WORKTHREAD_H
#define USB_WORKTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <libusb-1.0/libusb.h>
#include <QElapsedTimer>

#define VID 0x04d8
#define PID 0x0053
#define DEV_INTF 0
#define EP_OUT 0x01
#define EP_IN 0x81

class UsbWorkThread : public QThread
{
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);
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
    void USB_StopReceive();
    void USB_StopTransmit();

    // Init 'states'
    typedef enum {
        INIT = 0, OPEN_DEV, DEV_TAKE_CONTROL, CLAIM_INTERFACE, TRANSMIT_RECEIVE_INIT, INIT_END
    } UsbInitState;

    UsbInitState s = UsbInitState::INIT;

    uint8_t UserRxBuffer[65536];
    uint8_t UserTxBuffer[65536];

    bool start_transmit = false;
    bool start_receive = false;

    int UserRxBuffer_len = 0;
    int UserTxBuffer_len = 0;

    bool usb_receiver_drop = false;     // Drop any received usb data when this flag is active

    const int timeoutUsbInit_ms = 1000;

private:
    void run() override;

    bool usbInitialization();
    void USB_Reconnect();
    void USB_ReceiveTransmitInit();
    void USB_StartReceive(uint8_t *p_rx_buffer_offset);
    void USB_StartTransmit();
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

    void LIBUSB_CALL tx_callback(struct libusb_transfer *transfer);
    void LIBUSB_CALL rx_callback(struct libusb_transfer *transfer);


    libusb_context *context = nullptr;
    libusb_device_handle *handle = nullptr;

    struct libusb_transfer *t_rx = nullptr;
    struct libusb_transfer *t_tx = nullptr;

    bool m_quit = false;
    bool receive_in_progress = false;
    bool transmit_in_progress = false;

    int rx_complete_flag = 0;
    int tx_complete_flag = 0;

    QElapsedTimer console_spam_timer;
};

#endif // USB_WORKTHREAD_H
