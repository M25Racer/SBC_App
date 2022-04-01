#ifndef USB_GLOBAL_H
#define USB_GLOBAL_H

#include "QtCore"

#define VID 0x04d8
#define PID 0x0053
#define DEV_INTF 0
#define EP_OUT 0x01
#define EP_IN 0x81

const uint32_t USB_MAX_DATA_SIZE = (10*1024*1024);  // 10 MB

#endif // USB_GLOBAL_H
