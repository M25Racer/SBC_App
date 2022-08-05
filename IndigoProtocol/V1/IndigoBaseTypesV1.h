#pragma once

#if !defined (_MSC_VER)
    #include "stdint.h"
#endif

#define TRANSPORT_LEVEL_VERSION (1) 
#define COMMAND_LEVEL_VERSION   (1)

#define TRANSPORT_MIN_SIZE_HEADER (sizeof(TransportProtocolHeaderV1))
#define TRANSPORT_MAX_SIZE_FRAME  (500) //TODO
#define TRANSPORT_MAX_SIZE_BODY   ((TRANSPORT_MAX_SIZE_FRAME) - sizeof(TransportProtocolHeaderV1))

#define TRANSPORT_MAX_SIZE_PACKET (65535)
