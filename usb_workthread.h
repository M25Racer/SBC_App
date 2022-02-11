#ifndef USB_WORKTHREAD_H
#define USB_WORKTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <libusb-1.0/libusb.h>
#include <QSerialPort>
#include <QTimer>
#include<message_box.h>

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
    void usbInitTimeoutStart(const int timeout_ms);

private slots:
    //void closeSerialPort();
    void readDataSerialPort();
    void transmitDataSerialPort(const uint8_t *p_data, const int length);
    void handleError(QSerialPort::SerialPortError error);
    void timeoutSerialPortRx();
    void timeoutSerialPortReconnect();
    void postTxDataSTM(const uint8_t *p_data, const int length);
    //bool openSerialPort();

public:
    explicit UsbWorkThread(QObject *parent = nullptr);
    ~UsbWorkThread();

    bool openSerialPort();
    void closeSerialPort();

    void USB_ReceiveTransmitInit();
    void USB_StartReceive(uint8_t *p_rx_buffer_offset);
    void USB_StartTransmit();
    void USB_StopReceive();
    void USB_StopTransmit();
    void end();
    bool usbInitialization();
    void USB_Reconnect();
    void USB_Init();
    void USB_ThreadStart();

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

    struct libusb_transfer *t_rx = nullptr;
    struct libusb_transfer *t_tx = nullptr;

    // Init 'states'
    typedef enum {
        INIT = 0, OPEN_DEV, DEV_TAKE_CONTROL, CLAIM_INTERFACE, TRANSMIT_RECEIVE_INIT, INIT_END
    } UsbInitState;

    UsbInitState s = UsbInitState::INIT;

    libusb_context *context = nullptr;
    libusb_device_handle *handle = nullptr;

    uint8_t UserRxBuffer[65536];
    uint8_t UserTxBuffer[65536];

    int rx_complete_flag = 0;
    int tx_complete_flag = 0;

    bool receive_in_progress = false;
    bool transmit_in_progress = false;
    bool start_transmit = false;
    bool start_receive = false;

    int UserRxBuffer_len = 0;
    int UserTxBuffer_len = 0;

    bool usb_receiver_drop = false;     // Drop any received usb data when this flag is active

    MessageBox *m_message_box = nullptr;
    QSerialPort *m_serial = nullptr;

    QTimer *timerReconnect = nullptr;
    QTimer *timerRxTimeout = nullptr;

    bool timeoutSerialPort = false;

    // const int timeoutSerialPortRx_ms = 1;//10;              // timeout for serial port receiver (max time between rx packets)
    const int timeoutSerialPortReconnect_ms = 1000;
    const int timeoutUsbInit_ms = 1000;

    uint8_t quick_answer[2] = {0x00, 0x1d};
    bool transmit_quick_answer = false;

    QByteArray TtyUserRxBuffer;

    quint64 TtyUserRxBuffer_MaxSize = 4096;
    quint64 TtyUserRxBuffer_len = 0;

private:
    void run() override;
    void serialPortRxCleanup();

//    QString m_portName;
//    QString m_request;
//    int m_waitTimeout = 0;
//    QMutex m_mutex;
//    QWaitCondition m_cond;
    bool m_quit = false;
};

#endif // USB_WORKTHREAD_H
