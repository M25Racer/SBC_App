#ifndef USB_TRANSMITTER_H
#define USB_TRANSMITTER_H

#include <QCoreApplication>
#include <libusb-1.0/libusb.h>

#define VID 0x04d8
#define PID 0x0053
#define DEV_INTF 0
#define EP_OUT 0x01
#define EP_IN 0x81

//#define SBC_PROTOCOL_LENGTH_SIZE    4

class UsbTransmitter : public QObject
{
public:
    Q_OBJECT

signals:
    void consolePutData(const QString &data, uint8_t priority);
    void postTxDataToSerialPort(const uint8_t *p_data, const int len);
    void usbInitTimeoutStart(const int timeout_ms);

public:
    void USB_ReceiveTransmitInit();
    void USB_StartReceive(uint8_t *p_rx_buffer_offset);
    void USB_StartTransmit();
    void USB_StopReceive();
    void USB_StopTransmit();
    void end();
    bool usbInitialization();
    void USB_Reconnect();
    void USB_Init();

    // Hack to use libusb's 'C'-callback functions in 'C++' project
    // Use 'user_data' to pass pointer to 'this'
    static void LIBUSB_CALL static_tx_callback(struct libusb_transfer *transfer)
    {
        return reinterpret_cast<UsbTransmitter*>(transfer->user_data)->tx_callback(transfer);
    }
    static void LIBUSB_CALL static_rx_callback(struct libusb_transfer *transfer)
    {
        return reinterpret_cast<UsbTransmitter*>(transfer->user_data)->rx_callback(transfer);
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

    const int timeoutUsbInit_ms = 1000;

protected:

private:
};

#endif // USB_TRANSMITTER_H
