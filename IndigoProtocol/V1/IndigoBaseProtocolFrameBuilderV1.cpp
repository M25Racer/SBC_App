#include <QtGlobal>
//#include "StdAfx.h"
#if !defined (_MSC_VER)
    #include "memory.h"
#endif

#include "IndigoBaseProtocolFrameBuilderV1.h"
#include "crc16.h"
#include "crc8.h"

IndigoBaseProtocolFrameBuilderV1::IndigoBaseProtocolFrameBuilderV1()
{
}

IndigoBaseProtocolFrameBuilderV1::~IndigoBaseProtocolFrameBuilderV1()
{
}

void IndigoBaseProtocolFrameBuilderV1::SetAddress(const ToolAddress *source, const ToolAddress *dest)
{
    m_address_src = *source;
    m_address_dest = *dest;
}

void IndigoBaseProtocolFrameBuilderV1::SetIncPacket(bool enable)
{
    m_inc_packet = enable;
}

void IndigoBaseProtocolFrameBuilderV1::SetNotify(IndigoBaseProtocolFrameBuilderNotify notify, const void *param)
{
    m_notify = notify;
    m_notify_param = param;
}

bool IndigoBaseProtocolFrameBuilderV1::Create(const uint8_t *body, uint32_t flags, uint32_t length, uint32_t offset)
{
    if (m_inc_packet)
    {
        m_packet_num++;
        m_inc_packet = false;
    }


    m_last_error = LAST_ERROR_SUCCESS;

    memset(&m_header, 0, sizeof(m_header));
    // инициализируем заголовок пакета
    m_header.version = TRANSPORT_LEVEL_VERSION;
    m_header.header_length = sizeof(TransportProtocolHeaderV1);

    m_header.address_source = m_address_src;
    m_header.address_destination = m_address_dest;
    m_header.frame_num = m_packet_num;

    m_header.flags       = flags;
    m_header.data_length = length;
    m_header.data_offset = offset;

    m_header.crc8_header = calc_crc8((const uint8_t *)&m_header, offsetof(TransportProtocolHeaderV1, crc8_header));
    m_header.data_crc = calc_crc16(body, length);
    
    bool r = true;
    if (m_notify)
        r = m_notify(m_notify_param, (const TransportProtocolHeaderV1 *)&m_header, body);
    return r;
}

uint32_t IndigoBaseProtocolFrameBuilderV1::GetLastError() const
{
    return m_last_error;
}
