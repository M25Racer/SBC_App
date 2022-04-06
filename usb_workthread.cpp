#include "usb_workthread.h"
#include <message_box.h>
#include "../SRP_HS_USB_PROTOCOL/SRP_HS_USB_Protocol.h"

/* Extern global variables */
extern RingBuffer *m_ring;              // ring data buffer (ADC data) for QAM decoder
extern QWaitCondition ringNotEmpty;
extern QMutex m_mutex;

/* Global variables */
uint8_t UserRxBuffer[USB_MAX_DATA_SIZE];
uint8_t AdcDataBuffer[USB_MAX_DATA_SIZE];

UsbWorkThread::UsbWorkThread(QObject *parent) :
    QThread(parent)
{
    // Init rx transfers
    for(uint32_t i = 0; i < n_RxTransfers; ++i)
    {
        t_rx[i] = nullptr;
    }

    // Init rx parameters
    m_asyncParams.pData = (uint8_t*)&UserRxBuffer;
    m_asyncParams.dataOffset = 0;
    m_asyncParams.oneTransferLen = USB_BULK_SIZE;
    m_asyncParams.actualReceived = 0;
    m_asyncParams.dataSizeInBytes = sizeof(UserRxBuffer);
}

UsbWorkThread::~UsbWorkThread()
{
    m_quit = true;
    wait();
}

void UsbWorkThread::USB_ThreadStart()
{
    if(!isRunning())
    {
        m_quit = false;

        // Start thread
        start(QThread::HighestPriority);
    }
    else
    {
        //m_quit = false;
        //m_cond.wakeOne();
    }
}

void UsbWorkThread::run()
{
    struct timeval tv = {0, 0};

    console_spam_timer.start();

    int rc = 0;

    while (!m_quit)
    {
        if(start_transmit)
        {
            start_transmit = false;
            USB_StartTransmit();
        }

        if(start_receive)
        {
            if(!usb_receiver_drop)
            {
                // Start or continue receive
                emit consolePutData("USB start receiving\n", 0);
                start_receive = false;
                USB_StartReceive();
            }
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
            /* Handle Events */
            if(context != nullptr)
            {
                // If a zero timeval is passed, this function will handle any already-pending events and then immediately return in non-blocking style.
                rc = libusb_handle_events_timeout_completed(context, &tv, nullptr);

                if(rc != LIBUSB_SUCCESS)
                {
                    emit consolePutData(QString("Transfer Error: %1\n").arg(libusb_error_name(rc)), 1);
                }
            }
        }
    }
}

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

    for(uint32_t i = 0; i < n_RxTransfers; ++i)
    {
        if(t_rx[i] != nullptr)
        {
            emit consolePutData(QString("Error USB_ReceiveTransmitInit reported t_rx != nullptr, #%1\n").arg(i), 1);
            return;
        }

        t_rx[i] = libusb_alloc_transfer(n_iso_packets);   // Transfers intended for non-isochronous endpoints (e.g. control, bulk, interrupt) should specify an iso_packets count of zero.

        if(t_rx[i] == nullptr)
        {
            emit consolePutData(QString("Error USB_ReceiveTransmitInit: unable to allocate a libusb transfer rx, #%1\n").arg(i), 1);
            return;
        }
        else
        {
            // Filling
            libusb_fill_bulk_transfer(t_rx[i], handle, EP_IN, static_cast<uint8_t*>(UserRxBuffer) + i*USB_BULK_SIZE, USB_BULK_SIZE, static_rx_callback, this, 0);
        }
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

void UsbWorkThread::USB_StartReceive()
{
    int rc = 0; // result

    if(handle == nullptr)
    {
        emit consolePutData("Error USB_StartReceive reported handle = nullptr\n", 1);
        return;
    }

    for(uint32_t i = 0; i < n_RxTransfers; ++i)
    {
        if(t_rx[i] == nullptr)
        {
            emit consolePutData("Error USB_StartReceive transfer = NULL\n", 1);
            return;
        }
    }

    m_asyncParams.dataOffset = 0;
    m_asyncParams.oneTransferLen = USB_BULK_SIZE;
    m_asyncParams.actualReceived = 0;

    for(uint32_t i = 0; i < n_RxTransfers; ++i)
    {
        // Set rx buffer
        t_rx[i]->buffer = m_asyncParams.pData + m_asyncParams.dataOffset;
        m_asyncParams.dataOffset += m_asyncParams.oneTransferLen;

        // Submission
        rc = libusb_submit_transfer(t_rx[i]);

        if(rc < 0)
        {
            emit consolePutData(QString("Error USB_StartReceive libusb_submit_transfer %1: %2\n").arg(i).arg(libusb_error_name(rc)), 1);

            if (rc == LIBUSB_ERROR_NO_DEVICE)
            {
                // The device has been disconnected, try to reconnect
                USB_Reconnect();
            }

            return;
        }
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
    for(uint32_t i = 0; i < n_RxTransfers; ++i)
    {
        if(t_rx[i] == nullptr)
        {
            continue;
        }

        int rc = libusb_cancel_transfer(t_rx[i]);
        if(rc == 0)
        {
            // Cancel success
            emit consolePutData(QString("Usb canceled ongoing receive transfer, #%1\n").arg(i), 0);
        }

        if(rc < 0)
        {
            if(rc == LIBUSB_ERROR_NOT_FOUND)
            {
                // The transfer is not in progress, already complete, or already cancelled.
                continue;
            }

            // Some other error
            emit consolePutData(QString("Error USB_StopReceive libusb_cancel_transfer error: %1, #%2\n").arg(libusb_error_name(rc)).arg(i), 1);
        }
    }

    m_asyncParams.dataOffset = 0;
    m_asyncParams.actualReceived = 0;
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

void UsbWorkThread::USB_Deinit()
{
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int rc = 0;

    m_quit = true;    // stop usb thread 'main' cycle
    receive_in_progress = false;
    transmit_in_progress = false;
    start_transmit = false;
    start_receive = false;
    tx_complete_flag = 0;
    usb_receiver_drop = true;

    USB_StopReceive();
    libusb_handle_events_timeout_completed(context, &tv, nullptr);

    USB_StopTransmit();
    libusb_handle_events_timeout_completed(context, &tv, nullptr);

    // Deregister hotplug event callback
    libusb_hotplug_deregister_callback(context, h_hotplug);

    // Deallocation
    for(uint32_t i = 0; i < n_RxTransfers; ++i)
    {
        if(t_rx[i] != nullptr)
        {
            libusb_free_transfer(t_rx[i]);
            t_rx[i] = nullptr;
        }
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
        d.append("\n");
        emit consolePutData(d, 0);
        d.clear();

        // TODO add code to transmit more here?
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

int LIBUSB_CALL UsbWorkThread::hotplug_callback(libusb_context *ctx, libusb_device *device, libusb_hotplug_event event, void *user_data)
{
    Q_UNUSED(ctx);
    Q_UNUSED(device);
    Q_UNUSED(event);
    Q_UNUSED(user_data);

    //if(event & LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED)
    //    emit consolePutData("USB hotplug callback DEVICE_ARRIVED\n", 1);

    //if(event & LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT)
    //    emit consolePutData("USB hotplug callback DEVICE_LEFT\n", 1);

    // Returning 1 will cause this callback to be deregistered
    return 0;
}

void LIBUSB_CALL UsbWorkThread::rx_callback(struct libusb_transfer *transfer)
{
    if(transfer == nullptr)
    {
        emit consolePutData("Error rx_callback reported transfer = NULL\n", 1);
        return;
    }

    // Drop any received data from USB if needed
    if(usb_receiver_drop)
    {
        emit consolePutData("USB dropped received data, start receiving again\n", 0);
        USB_StopReceive();
        start_receive = true;
        return;
    }

    // Log received data
    emit consolePutData(QString("Transfer completed, actual received length %1\n").arg(transfer->actual_length), 0);
    uint8_t len_cut = transfer->actual_length > 16 ? 16 : uint8_t(transfer->actual_length);
    if(len_cut)
    {
        QString d;
        for(uint8_t i = 0; i < len_cut; ++i)
            d.append(QString("%1 ").arg(transfer->buffer[i], 2, 16, QLatin1Char('0')));
        if(len_cut < transfer->actual_length)
            d.append("...");
        d.append("\n");
        emit consolePutData(d, 0);
    }

    switch(transfer->status)
    {
    /** Transfer completed without error. Note that this does not indicate
     * that the entire amount of requested data was transferred. */
    case LIBUSB_TRANSFER_COMPLETED:
    {
        // If we waited for more data and timeout expired
        if(m_asyncParams.actualReceived && rx_timeout_timer.hasExpired(rx_timeout_ms))
        {
            // Drop previously received data
            emit consolePutData(QString("Usb receive timeout detected, broken previously received packet\n"), 1);
            //todo p_StartAddress ?
            USB_StopReceive();
            start_receive = true;
            return;
        }

        m_asyncParams.actualReceived += transfer->actual_length;    // max = USB_BULK_SIZE

        USBheader_t *header = (USBheader_t*)&UserRxBuffer;

        // If packet is too big already, start parsing it
        if(m_asyncParams.actualReceived >= m_asyncParams.dataSizeInBytes)
        {
            m_asyncParams.actualReceived = m_asyncParams.dataSizeInBytes;

            if(header->packet_length > uint32_t(m_asyncParams.actualReceived))
            {
                emit consolePutData(QString("Parse error, packet length is too long %1\n").arg(header->packet_length), 1);
                USB_StopReceive();
                start_receive = true;
                return;
            }
        }

        if(header->packet_length <= uint32_t(m_asyncParams.dataSizeInBytes))
        {
            // Packet length should be at least equal to header size
            if(header->packet_length < sizeof(USBheader_t))
            {
                emit consolePutData(QString("Parse error, packet length is too short %1\n").arg(header->packet_length), 1);
                USB_StopReceive();
                start_receive = true;
                return;
            }

            switch(header->type)
            {
                case SRP_LS_DATA:
                    // Retransmit data to PC
                    emit postTxDataToSerialPort(UserRxBuffer + sizeof(USBheader_t), header->packet_length - sizeof(USBheader_t));
                    emit consolePutData(QString("Received SRP LS DATA\n"), 0);
                    break;
                case SRP_HS_DATA:
                    emit consolePutData(QString("Received SRP HS DATA, continue to parsing\n"), 0);
                    parseHsData();
                    break;
            }

            // Packet received and parsed, stop any ongoing rx transfers
            USB_StopReceive();
            start_receive = true;
            return;
        }
        else
        {
            // Continue receive
            emit consolePutData(QString("Continue usb receive, start rx timeout timer\n"), 0);
            rx_timeout_timer.restart();

            // Start another transfer
            if(m_asyncParams.dataOffset >= m_asyncParams.dataSizeInBytes)
            {
                emit consolePutData(QString("Error continue usb receive: rx buffer overflow\n"), 0);
                USB_StopReceive();
                start_receive = true;
                return;
            }

            // Move data pointer
            transfer->buffer = m_asyncParams.pData + m_asyncParams.dataOffset;
            m_asyncParams.dataOffset += m_asyncParams.oneTransferLen;

            // Start transfer
            libusb_submit_transfer(transfer);
            return;
        }
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
        if(transfer->status != LIBUSB_TRANSFER_CANCELLED)
        {
            // Transfer error, stop ongoing rx transfers, start again
            USB_StopReceive();
            start_receive = true;

            // Check time, we should not spam to console too often
            if(console_spam_timer.hasExpired(1000))
            {
                emit consolePutData(QString("Error rx transfer: %1\n").arg(libusb_error_name(transfer->status)), 1);
                console_spam_timer.start();
            }
        }
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

    // Register 'hotplug event'
    if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
    {
        rc = libusb_hotplug_register_callback(context,
                                             libusb_hotplug_event(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT),
                                             LIBUSB_HOTPLUG_ENUMERATE,
                                             VID,
                                             PID,
                                             LIBUSB_HOTPLUG_MATCH_ANY,
                                             static_hotplug_callback,
                                             this,
                                             &h_hotplug);
        if(rc < 0)
        {
            emit consolePutData(QString("Error libusb_hotplug_register_callback: %1\n").arg(libusb_error_name(rc)), 1);
        }
    }

    return true;
}

void UsbWorkThread::USB_Init()
{
    bool r = false;
    int rc = 0;

    switch(InitState)
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

            InitState = UsbWorkThread::OPEN_DEV;
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

            InitState = UsbWorkThread::DEV_TAKE_CONTROL;
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

            InitState = UsbWorkThread::CLAIM_INTERFACE;
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

            InitState = UsbWorkThread::TRANSMIT_RECEIVE_INIT;
            [[fallthrough]];
            /* no break */

        case UsbWorkThread::TRANSMIT_RECEIVE_INIT:
            // Recieve & transmit init
            USB_ReceiveTransmitInit();

            // Start receiving
            emit consolePutData("Start receiving for the first time\n", 0);
            start_receive = true;
            usb_receiver_drop = false;
            InitState = UsbWorkThread::INIT_END;
            [[fallthrough]];
            /* no break */
        case UsbWorkThread::INIT_END:
            // Start LibUSB polling thread
            USB_ThreadStart();
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

    USB_Deinit();
    InitState = UsbWorkThread::INIT;
    USB_Init();
}

// SRP HS protocol handling (TODO: move to separate class)

void UsbWorkThread::sendHsCommand(uint8_t Command, uint32_t Length, uint8_t *p_Data)
{
    USBheader_t header;
    header.cmd = Command;
    header.type = SRP_HS_DATA;
    header.packet_length = sizeof(USBheader_t) + Length;

    emit consolePutData("sendHsCommand\n", 1);

    if(Length)
    {
        if(p_Data == nullptr)
        {
            emit consolePutData("Error: sendHsCommand reported p_data == null\n", 1);
            return;
        }

        if(header.packet_length > sizeof(UserTxBuffer))
        {
            emit consolePutData("Error: sendHsCommand reported length is too long\n", 1);
            header.packet_length = sizeof(UserTxBuffer);
        }
    }

    // Cancel ongoing transfer (if any)
    USB_StopTransmit();

    UserTxBuffer_len = header.packet_length;
    memcpy(UserTxBuffer, (uint8_t*)&header, sizeof(header));

    if(Length)
        memcpy(UserTxBuffer + sizeof(header), p_Data, Length);

    emit consolePutData("Transmit to H7 " + QString::number(UserTxBuffer_len) + " bytes\n", 0);
    start_transmit = true;
}

void UsbWorkThread::parseHsData()
{
    USBheader_t *header = (USBheader_t*)&UserRxBuffer[0];
    uint8_t *p_data = (uint8_t*)&UserRxBuffer[sizeof(USBheader_t)];

    switch(header->cmd)
    {
        case USB_CMD_ADC_START:
        case USB_CMD_ADC_STOP:
            break;

        case USB_CMD_GET_DATA_SIZE:
            stm32_ready_data_size = UserRxBuffer[sizeof(USBheader_t)];
            stm32_ready_data_size |= (uint32_t)UserRxBuffer[sizeof(USBheader_t) + 1] << 8;
            stm32_ready_data_size |= (uint32_t)UserRxBuffer[sizeof(USBheader_t) + 2] << 16;
            stm32_ready_data_size |= (uint32_t)UserRxBuffer[sizeof(USBheader_t) + 3] << 24;
            emit consolePutData(QString("parseHsData(): Ready ADC data size = %1\n").arg(stm32_ready_data_size), 0);
            break;

        case USB_CMD_GET_DATA:
        {
            emit consolePutData(QString("parseHsData(): USB_CMD_GET_DATA\n"), 0);
            memcpy(AdcDataBuffer, UserRxBuffer + sizeof(USBheader_t), header->packet_length - sizeof(USBheader_t));

            bool res = m_ring->Append(UserRxBuffer + sizeof(USBheader_t), header->packet_length - sizeof(USBheader_t));
            if(res)
            {
                // Wake threads waiting for 'wait condiion'
                ringNotEmpty.wakeAll();
            }
            else
            {
                // Error: can't add new data to ring buffer
                emit consolePutData("Error: unable to add new adc data to ring buffer\n", 1);
            }

            emit consoleAdcFile(AdcDataBuffer, header->packet_length - sizeof(USBheader_t));
            break;
        }

        case USB_CMD_GET_STATUS:
        {
            stm32_ready_data_size = 0;
            uint8_t AdcStatus = *p_data;

            if(m_asyncParams.actualReceived >= (int)sizeof(USBheader_t) + 1 + 4)
            {
                stm32_ready_data_size = UserRxBuffer[sizeof(USBheader_t) + 1];
                stm32_ready_data_size |= (uint32_t)UserRxBuffer[sizeof(USBheader_t) + 2] << 8;
                stm32_ready_data_size |= (uint32_t)UserRxBuffer[sizeof(USBheader_t) + 3] << 16;
                stm32_ready_data_size |= (uint32_t)UserRxBuffer[sizeof(USBheader_t) + 4] << 24;
                emit consolePutData(QString("parseHsData(): Ready ADC data size = %1\n").arg(stm32_ready_data_size), 0);
            }

            if(AdcStatus == ADC_STATUS_READY)
            {
                emit consolePutData("parseHsData(): ADC status = ready\n", 0);

                // If there is an ADC data available to read
                if(stm32_ready_data_size)
                {
                    // Send GET_DATA command
                    sendHsCommand(USB_CMD_GET_DATA, sizeof(stm32_ready_data_size), (uint8_t*)&stm32_ready_data_size);
                }
            }
            else if(AdcStatus == ADC_STATUS_BUSY)
            {
                emit consolePutData("parseHsData(): ADC status = busy\n", 0);
            }
            else
            {
                emit consolePutData("parseHsData(): ADC status = unknown\n", 0);
            }
            break;
        }
    }
}
