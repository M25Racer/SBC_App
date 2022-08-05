#pragma once

#if !defined (_MSC_VER)
#include "stdlib.h"
#endif

#include "IndigoBaseTypesV1.h"
#include "TransportProtocolHeader.h"

typedef bool(*IndigoBaseProtocolFrameBuilderNotify) (const void *param, const TransportProtocolHeaderV1 *header, const uint8_t *body);

class IndigoBaseProtocolFrameBuilderV1
{
public:
    IndigoBaseProtocolFrameBuilderV1();
    ~IndigoBaseProtocolFrameBuilderV1();

    void SetAddress(const ToolAddress *source, const ToolAddress *dest);
    void SetIncPacket(bool enable);
    void SetNotify(IndigoBaseProtocolFrameBuilderNotify notify, const void *param);

    bool Create(const uint8_t *body, uint32_t flags, uint32_t length, uint32_t offset);

    uint32_t GetLastError() const;


private:
    enum constants
    {
        MAX_BUFFER = TRANSPORT_MAX_SIZE_FRAME,

        LAST_ERROR_SUCCESS = 0,
        LAST_ERROR_NOT_ENOUGH_MEMORY,
        LAST_ERROR_NULL_POINTER,
        LAST_ERROR_NOTIFY,
    };

private:
    uint8_t m_packet_num = 0;
    bool m_inc_packet = true;
 
    uint32_t m_last_error = LAST_ERROR_SUCCESS;
 
    ToolAddress m_address_src;
    ToolAddress m_address_dest;
   
    TransportProtocolHeaderV1 m_header;
    IndigoBaseProtocolFrameBuilderNotify m_notify = NULL;
    const void  *m_notify_param = NULL;
};