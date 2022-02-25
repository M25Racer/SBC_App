#include "usb_workthread.h"

//#include <QSerialPort>
//#include <QTime>

UsbWorkThread::UsbWorkThread(QObject *parent) :
    QThread(parent)
{
}

UsbWorkThread::~UsbWorkThread()
{
//    m_mutex.lock();
    m_quit = true;
//    m_cond.wakeOne();
//    m_mutex.unlock();
    wait();
}

//void UsbWorkThread::transaction(const QString &portName, int waitTimeout, const QString &request)
//{
//    const QMutexLocker locker(&m_mutex);
//    m_portName = portName;
//    m_waitTimeout = waitTimeout;
//    m_request = request;

//    if (!isRunning())
//        start();
//    else
//        m_cond.wakeOne();
//}
void UsbWorkThread::USB_ThreadStart()
{
    //    const QMutexLocker locker(&m_mutex);
    //    m_portName = portName;
    //    m_waitTimeout = waitTimeout;
    //    m_request = request;

    if(!isRunning())
        start();
//    else
//        m_cond.wakeOne();
}


void UsbWorkThread::run()
{
    struct timeval tv = {0, 0};

    while (!m_quit)
    {
        int rc = 0;

        if(start_transmit)
        {
            start_transmit = false;
            USB_StartTransmit();
        }

        if(start_receive)
        {
            // Start or continue receive
            start_receive = false;
            USB_StartReceive(&UserRxBuffer[UserRxBuffer_len]);
        }

        if(transmit_in_progress)
        {
            if(!tx_complete_flag)
            {
                /* Handle Events */
                if(context != nullptr)
                {
                    // If a zero timeval is passed, this function will handle any already-pending events and then immediately return in non-blocking style.
                    rc = libusb_handle_events_timeout_completed(context, &tv, nullptr);

                    if(rc != LIBUSB_SUCCESS)
                    {
                        emit consolePutData(QString("Usb Transfer Error: %1\n").arg(libusb_error_name(rc)), 1);
                    }
                }
            }

            if(tx_complete_flag)
            {
                transmit_in_progress = false;
            }
        }

        if(receive_in_progress)
        {
            if(!rx_complete_flag)
            {
                /* Handle Events */
                if(context != nullptr)
                {
                    // If a zero timeval is passed, this function will handle any already-pending events and then immediately return in non-blocking style.
                    rc = libusb_handle_events_timeout_completed(context, &tv, nullptr);

                    if(rc != LIBUSB_SUCCESS)
                    {
                        emit consolePutData(QString("Transfer Error: %1\n").arg(libusb_error_name(rc)), 1);
                        //fprintf(stderr, "Transfer Error: %s\n", libusb_error_name(rc));
                        //break;
                    }
                }
            }

            if(rx_complete_flag)
            {
                receive_in_progress = false;
            }
        }
    }
}

#include <message_box.h>
#include "../SRP_HS_USB_PROTOCOL/SRP_HS_USB_Protocol.h"

///*Спецификация USB определяет 4 типа потоков данных:
// 1. bulk  transfer  -  предназначен  для  пакетной  передачи  данных с
//    размером  пакетов  8,  16,  32,  64 для USB 1.1 и 512 для USB 2.0.
//    Используется алгоритм перепосылки (в случае возникновения ошибок),
//    а  управление  потоком  осуществляется  с использованием handshake
//    пакетов,  поэтому  данный тип является достоверным. Поддерживаются
//    оба направления - IN и OUT.
// 2. control  transfer - предназначен для конфигурирования и управления
//    устройством.   Также,   как   и  в  bulk,  используются  алгоритмы
//    подтверждения   и   перепосылки,  поэтому  этот  тип  обеспечивает
//    гарантированный   обмен  данными.  Направления  -  IN  (status)  и
//    OUT(setup, control).
// 3. interrupt  transfer  -  похож  на bulk. Размер пакета - от 1 до 64
//    байт для USB 1.1 и до 1024 байт для USB 2.0. Этот тип гарантирует,
//    что устройство будет опрашиваться (то есть хост будет отсылать ему
//    token "IN") хостом с заданным интервалом. Направление - IN.
// 4. isochronous  transfer  -  предназначен  для  передачи  данных  без
//    управления  потоком  (без  подтверждений).  Область  применения  -
//    аудио-потоки,  видео-потоки.  Размер пакета - до 1023 байт для USB
//    1.1  и  до 1024 байт для USB 2.0. Предусмотрен контроль ошибок (на
//    приемной стороне) по CRC16. Направления - IN и OUT.

// Asynchronous receive
void UsbWorkThread::USB_ReceiveTransmitInit()
{
    int n_iso_packets = 0;      // number of isochronous packet descriptors

    memset(UserRxBuffer, 0, sizeof(UserRxBuffer));

    if(t_rx != nullptr)
    {
        emit consolePutData("Error USB_ReceiveTransmitInit reported t_rx != nullptr\n", 1);
        return;
    }

    t_rx = libusb_alloc_transfer(n_iso_packets);   // Transfers intended for non-isochronous endpoints (e.g. control, bulk, interrupt) should specify an iso_packets count of zero.

    if(t_rx == nullptr)
    {
        emit consolePutData("Error USB_ReceiveTransmitInit: unable to allocate a libusb transfer rx\n", 1);
        return;
    }
    else
    {
        // Filling
        libusb_fill_bulk_transfer(t_rx, handle, EP_IN, static_cast<uint8_t*>(UserRxBuffer), sizeof(UserRxBuffer), static_rx_callback, this, 0);
    }


    if(t_tx != nullptr)
    {
        emit consolePutData("Error USB_ReceiveTransmitInit reported t_tx != nullptr\n", 1);
        return;
    }

    t_tx = libusb_alloc_transfer(n_iso_packets);   // Transfers intended for non-isochronous endpoints (e.g. control, bulk, interrupt) should specify an iso_packets count of zero.

    if(t_tx == nullptr)
    {
        emit consolePutData("Error USB_ReceiveTransmitInit: unable to allocate a libusb transfer tx\n", 1);
        return;
    }
    else
    {
        // Filling
        libusb_fill_bulk_transfer(t_tx, handle, EP_OUT, UserTxBuffer, UserTxBuffer_len, static_tx_callback, this, 1000);
    }
}

void UsbWorkThread::USB_StartReceive(uint8_t *p_rx_buffer_offset)
{
    int rc = 0; // result

    if(t_rx == nullptr)
    {
        emit consolePutData("Error USB_StartReceive transfer = NULL\n", 1);
        return;
    }

    if(handle == nullptr)
    {
        emit consolePutData("Error USB_StartReceivereported handle = nullptr\n", 1);
        return;
    }

    // Reset completion flag
    rx_complete_flag = 0;

    // Set rx buffer
    t_rx->buffer = p_rx_buffer_offset;

    // Submission
    rc = libusb_submit_transfer(t_rx);

    if(rc < 0)
    {
        emit consolePutData(QString("Error USB_StartReceive libusb_submit_transfer: %1\n").arg(libusb_error_name(rc)), 1);

        if (rc == LIBUSB_ERROR_NO_DEVICE)
        {
            // The device has been disconnected, try to reconnect
            USB_Reconnect();
        }

        return;
    }

    receive_in_progress = true;
}

void UsbWorkThread::USB_StartTransmit()
{
    int rc = 0;

    if(t_tx == nullptr)
    {
        emit consolePutData("Error USB_StartTransmit reported t_tx == nullptr\n", 1);
        return;
    }

    if(handle == nullptr)
    {
        emit consolePutData("Error USB_StartTransmit reported handle = nullptr\n", 1);
        return;
    }

    // Filling
    tx_complete_flag = 0;
    t_tx->length = UserTxBuffer_len;

    // Submission
    rc = libusb_submit_transfer(t_tx);

    if(rc < 0)
    {
        emit consolePutData(QString("Error USB_StartTransmit libusb_submit_transfer: %1\n").arg(libusb_error_name(rc)), 1);

        if (rc == LIBUSB_ERROR_NO_DEVICE)
        {
            // The device has been disconnected, try to reconnect
            USB_Reconnect();
        }

        return;
    }

    transmit_in_progress = true;
}

void UsbWorkThread::USB_StopReceive()
{
    if(t_rx == nullptr)
    {
        return;
    }

    int rc = libusb_cancel_transfer(t_rx);
    if(rc == 0)
    {
        // Cancel success
        emit consolePutData("Usb canceled ongoing receive transfer\n", 0);
    }

    if(rc < 0)
    {
        if(rc == LIBUSB_ERROR_NOT_FOUND)
        {
            // The transfer is not in progress, already complete, or already cancelled.
            return;
        }

        // Some other error
        emit consolePutData(QString("Error USB_StopReceive libusb_cancel_transfer error: %1\n").arg(libusb_error_name(rc)), 1);
    }

    // Reset received length
    UserRxBuffer_len = 0;
}

void UsbWorkThread::USB_StopTransmit()
{
    if(t_tx == nullptr)
        return;

    int rc = libusb_cancel_transfer(t_tx);
    if(rc == 0)
    {
        // Cancel success
        emit consolePutData("Usb canceled ongoing transmit transfer\n", 0);
    }

    if(rc < 0)
    {
        if(rc == LIBUSB_ERROR_NOT_FOUND)
        {
            // The transfer is not in progress, already complete, or already cancelled.
        }
        else
        {
            // Some other error, report it
            emit consolePutData(QString("Error USB_StopTransmit libusb_cancel_transfer error: %1\n").arg(libusb_error_name(rc)), 1);
        }
    }
}

void UsbWorkThread::end()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int rc = 0;

    m_quit = true;    // stop usb thread 'main' cycle
    receive_in_progress = false;
    transmit_in_progress = false;
    start_transmit = false;
    rx_complete_flag = 0;
    tx_complete_flag = 0;
    usb_receiver_drop = true;

    USB_StopReceive();
    libusb_handle_events_timeout_completed(context, &tv, nullptr);

    USB_StopTransmit();
    libusb_handle_events_timeout_completed(context, &tv, nullptr);

    // Deallocation
    if(t_rx != nullptr)
    {
        libusb_free_transfer(t_rx);
        t_rx = nullptr;
    }

    if(t_tx != nullptr)
    {
        libusb_free_transfer(t_tx);
        t_tx = nullptr;
    }

    if(handle)
    {
        rc = libusb_release_interface(handle, DEV_INTF);
        if(rc < 0)
            emit consolePutData(QString("Error libusb_release_interface: %1\n").arg(libusb_error_name(rc)), 1);
    }
    if(handle)
    {
        rc = libusb_attach_kernel_driver(handle, DEV_INTF);
        if(rc < 0)
            emit consolePutData(QString("Error libusb_attach_kernel_driver: %1\n").arg(libusb_error_name(rc)), 1);
    }
    if(handle)
    {
        libusb_close(handle);
    }
    libusb_exit(nullptr);
}

/** \ingroup asyncio
 * Asynchronous transfer callback function type. When submitting asynchronous
 * transfers, you pass a pointer to a callback function of this type via the
 * \ref libusb_transfer::callback "callback" member of the libusb_transfer
 * structure. libusb will call this function later, when the transfer has
 * completed or failed. See \ref asyncio for more information.
 * \param transfer The libusb_transfer struct the callback function is being
 * notified about.
 */
void LIBUSB_CALL UsbWorkThread::tx_callback(struct libusb_transfer *transfer)
{
    // Transfer complete, unlock usb receiver
    usb_receiver_drop = false;

    if(transfer == nullptr)
    {
        emit consolePutData("Error tx_callback reported transfer = NULL\n", 1);
        return;
    }

    switch(transfer->status)
    {
    /** Transfer completed without error. Note that this does not indicate
     * that the entire amount of requested data was transferred. */
    case LIBUSB_TRANSFER_COMPLETED:
    {
        emit consolePutData(QString("Transfer completed, actual transmitted length %1\n").arg(transfer->actual_length), 0);
        uint8_t len_cut = transfer->actual_length > 254 ? 254 : uint8_t(transfer->actual_length);
        QString d;
        for(uint8_t i = 0; i < len_cut; ++i)
            d.append(QString("%1 ").arg(transfer->buffer[i], 2, 16, QLatin1Char('0')));
            //emit consolePutData(QString("%1").arg(transfer->buffer[i], 2, 16, QLatin1Char('0')));
        d.append("\n");
        emit consolePutData(d, 0);
        d.clear();

//todo add code to transmit more here
        //qDebug() << "Data 0 " << transfer->buffer[0];

//        UserRxBuffer_len += transfer->actual_length;

//        if((transfer->actual_length < maxPacketSize) || (UserRxBuffer_len >= int(sizeof(UserRxBuffer))))
//        {
//            if (UserRxBuffer[0] == SRP_ADDR)
//            {
//                message_box_srp(UserRxBuffer, uint16_t(UserRxBuffer_len), MASTER_ADDR, SRP_ADDR);
//                //change_baudrate(UserTxBufferHS[2]);
//            }

//            // TODO: add other addresses
//        }
//        else
//        {
//            transfer->buffer = &UserRxBuffer[UserRxBuffer_len];
//            libusb_submit_transfer(transfer);
//        }
    }
        break;

    /** Transfer failed */
    case LIBUSB_TRANSFER_ERROR:
        break;

    /** Transfer timed out */
    case LIBUSB_TRANSFER_TIMED_OUT:
        break;

    /** Transfer was cancelled */
    case LIBUSB_TRANSFER_CANCELLED:
        break;

    /** For bulk/interrupt endpoints: halt condition detected (endpoint
     * stalled). For control endpoints: control request not supported. */
    case LIBUSB_TRANSFER_STALL:
        break;

    /** Device was disconnected */
    case LIBUSB_TRANSFER_NO_DEVICE:
        break;

    /** Device sent more data than requested */
    case LIBUSB_TRANSFER_OVERFLOW:
        break;
    }

    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        emit consolePutData(QString("Error tx transfer: %1\n").arg(libusb_error_name(transfer->status)), 1);
    }

    tx_complete_flag = 1;
}

void LIBUSB_CALL UsbWorkThread::rx_callback(struct libusb_transfer *transfer)
{
    // Stop usb polling timer

    QString d;

    if(transfer == nullptr)
    {
        emit consolePutData("Error rx_callback reported transfer = NULL\n", 1);
        return;
    }

    // rx complete flag
    rx_complete_flag = 1;

    // Drop any received data from USB if needed
    if(usb_receiver_drop)
    {
        emit consolePutData("USB dropped received data\nStart receiving again\n", 0);
        start_receive = true;
        return;
    }

    switch(transfer->status)
    {
    /** Transfer completed without error. Note that this does not indicate
     * that the entire amount of requested data was transferred. */
    case LIBUSB_TRANSFER_COMPLETED:
    {
        emit consolePutData(QString("Transfer completed, actual received length %1\n").arg(transfer->actual_length), 0);
        uint8_t len_cut = transfer->actual_length > 254 ? 254 : uint8_t(transfer->actual_length);
        for(uint8_t i = 0; i < len_cut; ++i)
            d.append(QString("%1 ").arg(transfer->buffer[i], 2, 16, QLatin1Char('0')));
            //emit consolePutData(QString("%1").arg(transfer->buffer[i], 2, 16, QLatin1Char('0')));
        d.append("\n");
        emit consolePutData(d, 0);
        d.clear();

        UserRxBuffer_len += transfer->actual_length;

// Retransmit data to PC
emit consolePutData(QString("Received SRP LS DATA\n"), 0);
emit postTxDataToSerialPort(UserRxBuffer, UserRxBuffer_len);
UserRxBuffer_len = 0;

//        USBheader_t *header = (USBheader_t*)&UserRxBuffer;

//        // If packet is too big already, start parsing it
//        if(UserRxBuffer_len >= int(sizeof(UserRxBuffer)))
//        {
//            UserRxBuffer_len = sizeof(UserRxBuffer);

//            if(header->packet_length > uint32_t(UserRxBuffer_len))
//            {
//                emit consolePutData(QString("Parse error, packet length is too long %1\n").arg(header->packet_length), 1);
//                UserRxBuffer_len = 0;
//                break;
//            }
//        }

//        if(header->packet_length <= uint32_t(UserRxBuffer_len))
//        {
//            // Packet data should be at least 1 byte in length
//            if(header->packet_length < sizeof(USBheader_t) + 1)
//            {
//                emit consolePutData(QString("Parse error, packet length is too short %1\n").arg(header->packet_length), 1);
//                UserRxBuffer_len = 0;
//                break;
//            }

//            switch(header->type)
//            {
//                case SRP_LS_DATA:
//                    // Retransmit data to PC
//                    emit consolePutData(QString("Received SRP LS DATA\n"), 0);
//                    emit postTxDataToSerialPort(UserRxBuffer + sizeof(USBheader_t), header->packet_length - sizeof(USBheader_t));
//                    break;
//                case SRP_HS_DATA:
//                    // todo
//                    emit consolePutData(QString("Received SRP HS DATA, todo\n"), 0);
//                    break;
//            }

//            UserRxBuffer_len = 0;
//        }

////        // Old legacy code
////        if((transfer->actual_length < maxPacketSize) || (UserRxBuffer_len >= int(sizeof(UserRxBuffer))))
////        {
////            //todo add protocol STM32H7<->SBC parsing here
////            //emit parseReceivedUsbData(UserRxBuffer, UserRxBuffer_len);
////            UserRxBuffer_len = 0;
////        }
//        else
//        {
//            // Continue receive
//            emit consolePutData(QString("Continue usb receive\n"), 0);
//            start_receive = true;
//            //USB_StartReceive(&UserRxBuffer[UserRxBuffer_len]);
//        }
    }
        break;

    /** Transfer failed */
    case LIBUSB_TRANSFER_ERROR:
        break;

    /** Transfer timed out */
    case LIBUSB_TRANSFER_TIMED_OUT:
        break;

    /** Transfer was cancelled */
    case LIBUSB_TRANSFER_CANCELLED:
        break;

    /** For bulk/interrupt endpoints: halt condition detected (endpoint
     * stalled). For control endpoints: control request not supported. */
    case LIBUSB_TRANSFER_STALL:
        break;

    /** Device was disconnected */
    case LIBUSB_TRANSFER_NO_DEVICE:
        break;

    /** Device sent more data than requested */
    case LIBUSB_TRANSFER_OVERFLOW:
        break;
    }

    if(transfer->status != LIBUSB_TRANSFER_COMPLETED)
    {
        UserRxBuffer_len = 0;

        if(transfer->status != LIBUSB_TRANSFER_CANCELLED)
            emit consolePutData(QString("Error rx transfer: %1\n").arg(libusb_error_name(transfer->status)), 1);
    }

    // If we are not receiving already
    if(rx_complete_flag)
    {
        // Start receiving again
        emit consolePutData("Start receiving again\n", 0);
        start_receive = true;
    }
}

bool UsbWorkThread::usbInitialization()
{
    /* Initialize libusb */
    int rc = libusb_init(&context);
    if(rc < 0)
    {
        emit consolePutData(QString("Error Initializing libusb: %1\n").arg(libusb_error_name(rc)), 1);
        return false;
    }
    Q_ASSERT(rc == 0);

    /* Set debugging output to max level */
    rc = libusb_set_option(nullptr, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING);     // LIBUSB_LOG_LEVEL_DEBUG
    if(rc < 0)
    {
        emit consolePutData(QString("Error libusb_set_option: %1\n").arg(libusb_error_name(rc)), 1);
        return false;
    }

    /* List all usb */
    libusb_device **list = nullptr;
    ssize_t count = libusb_get_device_list(context, &list);
    Q_ASSERT(count > 0);

    for (ssize_t idx = 0; idx < count; ++idx)
    {
        libusb_device *device = list[idx];
        libusb_device_descriptor desc = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

        rc = libusb_get_device_descriptor(device, &desc);
        Q_ASSERT(rc == 0);

        emit consolePutData(QString("Vendor:Device = %1:%2\n")
                            .arg(desc.idVendor, 4, 16, QLatin1Char('0'))
                            .arg(desc.idProduct, 4, 16, QLatin1Char('0')), 1);
    }

    // Free device list
    libusb_free_device_list(list, 1);

    return true;
}

void UsbWorkThread::USB_Init()
{
    bool r = false;
    int rc = 0;

    switch(s)
    {
        case UsbWorkThread::INIT:
            r = usbInitialization();

            if(!r)
            {
                // Try again
                emit consolePutData(QString("Usb initialization failed, next attempt in %1 ms\n").arg(timeoutUsbInit_ms), 1);
                emit usbInitTimeoutStart(timeoutUsbInit_ms);
                return;
            }

            s = UsbWorkThread::OPEN_DEV;
            [[fallthrough]];
            /* no break */

        case UsbWorkThread::OPEN_DEV:
            handle = libusb_open_device_with_vid_pid(nullptr, VID, PID);

            if(handle != nullptr)
            {
                //printf("USB device %04x:%04x has been connected\n", VID, PID);
                emit consolePutData(QString("USB device %1:%2 has been connected\n")
                                   .arg(VID, 4, 16, QLatin1Char('0'))
                                   .arg(PID, 4, 16, QLatin1Char('0')), 1);
            }
            else
            {
                emit consolePutData(QString("USB device %1:%2 has not been connected\n")
                                   .arg(VID, 4, 16, QLatin1Char('0'))
                                   .arg(PID, 4, 16, QLatin1Char('0')), 1);
                if(handle)
                {
                    //m_usb->end(handle);
                    libusb_attach_kernel_driver(handle, DEV_INTF);
                    libusb_close(handle);
                }
                // Try again
                emit consolePutData(QString("Usb initialization failed, next attempt in %1 ms\n").arg(timeoutUsbInit_ms), 1);
                emit usbInitTimeoutStart(timeoutUsbInit_ms);
                return;
            }

            s = UsbWorkThread::DEV_TAKE_CONTROL;
            [[fallthrough]];
            /* no break */

        case UsbWorkThread::DEV_TAKE_CONTROL:
            // Забираем контроль у системы на это устройство (не поддерживается на Windows)
            rc = libusb_kernel_driver_active(handle, DEV_INTF);

            while(1)
            {
                if(rc == LIBUSB_SUCCESS)
                {
                    // No kernel driver is active - no need to detach
                    break;
                }

                if(rc == 1)
                {
                    libusb_detach_kernel_driver(handle, DEV_INTF);
                    break;
                }
#ifdef Q_OS_WIN
                else if(rc == LIBUSB_ERROR_NOT_SUPPORTED)
                {
                    break;
                }
#else
                else if(rc == LIBUSB_ERROR_NOT_SUPPORTED)
                {
                    //fprintf(stderr, "Error libusb_set_option: %s\n", libusb_error_name(rc));
                    emit consolePutData(QString("Error libusb_set_option: %1\n")
                                       .arg(libusb_error_name(rc)), 1);
                }
#endif
                else if(rc == LIBUSB_ERROR_NO_DEVICE)
                {
                    emit consolePutData("USB device has been disconnected\n", 1);
                    emit consolePutData(QString("Error libusb_kernel_driver_active: %1\n").arg(libusb_error_name(rc)), 1);
                }

                // Try again
                emit consolePutData(QString("Usb initialization failed, next attempt in %1 ms\n").arg(timeoutUsbInit_ms), 1);
                emit usbInitTimeoutStart(timeoutUsbInit_ms);
                return;
            }

            s = UsbWorkThread::CLAIM_INTERFACE;
            [[fallthrough]];
            /* no break */

        case UsbWorkThread::CLAIM_INTERFACE:
            // Захватываем интерфейс
            rc = libusb_claim_interface(handle, DEV_INTF);

            if(rc != 0)
            {
                emit consolePutData("Interface error", 1);
                emit consolePutData(QString("Error libusb_claim_interface: %1\n").arg(libusb_error_name(rc)), 1);

                // Try again
                emit consolePutData(QString("Usb initialization failed, next attempt in %1 ms\n").arg(timeoutUsbInit_ms), 1);
                emit usbInitTimeoutStart(timeoutUsbInit_ms);
            }

            s = UsbWorkThread::TRANSMIT_RECEIVE_INIT;
            [[fallthrough]];
            /* no break */

        case UsbWorkThread::TRANSMIT_RECEIVE_INIT:
            // Recieve & transmit init
            USB_ReceiveTransmitInit();

            // Start receiving
            emit consolePutData("Start receiving for the first time\n", 0);
            start_receive = true;
            s = UsbWorkThread::INIT_END;
            [[fallthrough]];
            /* no break */

        default:
            break;
    }
}

void UsbWorkThread::USB_Reconnect()
{
    emit consolePutData("USB reconnect attempt...\n", 1);

    USB_StopReceive();
    USB_StopTransmit();

    end();
    s = UsbWorkThread::INIT;
    USB_Init();
}
