#include <QtGlobal>
//#include "StdAfx.h"
#if !defined (_MSC_VER)
    #include "string.h"
    #include "stddef.h"
#endif
#include "IndigoBaseProtocolUsbBuilderV1.h"
#include "../IndigoBaseTypes.h"


IndigoProtocolUsbBuilderV1::IndigoProtocolUsbBuilderV1() : 
m_last_err(LAST_ERROR_SUCCESS),
m_notify(NULL),
m_param(NULL),
m_packet_num(0)
{
    memset(&m_header, 0, sizeof(m_header));
}

uint32_t IndigoProtocolUsbBuilderV1::GetLastError() const
{
    return m_last_err;
}

void IndigoProtocolUsbBuilderV1::SetAddress(const ToolAddress *address_src, const ToolAddress *address_dest)
{
    if (!address_src || !address_dest)
        return;
    
    m_header.address_source = *address_src;
    m_header.address_destination = *address_dest;
}

void IndigoProtocolUsbBuilderV1::SetNotify(IndigoBaseProtocolBuilderNotify notify, const void *param)
{
    m_param = param;
    m_notify = notify;
}

void IndigoProtocolUsbBuilderV1::Initialize()
{
    memset(&m_header, 0, sizeof(m_header));
}

void IndigoProtocolUsbBuilderV1::AddOffsets(CmdRepeatOffset *offsets, uint32_t count)
{
    Q_UNUSED(offsets);
    Q_UNUSED(count);
}

void IndigoProtocolUsbBuilderV1::SetDataLength(uint32_t data_length)
{
    m_header.data_length = data_length;
    m_header.data_offset = 0;
}

bool IndigoProtocolUsbBuilderV1::BodyAdd(const uint8_t *body, uint32_t size)
{
	CreateHeader();

    bool res = true;
    if (m_notify)
        res = m_notify(m_param, body, size);
    return res;
}

bool IndigoProtocolUsbBuilderV1::BodyEnd()
{
    memset(&m_header, 0, sizeof(m_header));
    return true;
}

bool IndigoProtocolUsbBuilderV1::CreateHeader()
{
    //  если выставлен header length значит хидер сформирован и отослан 
    if (m_header.header_length != 0)
        return true;

    m_header.version = 1;
    m_header.header_length = sizeof(TransportProtocolHeaderV1);
    //m_header.address_source ;      // адреса источника и приемника заданы выше
    //m_header.address_destination ;

    m_header.frame_num = m_packet_num++;
    m_header.flags = 0;       // заголовок всегда один, фрагментации нет

    //m_header.data_length;     // размер данных задан выше
    m_header.data_offset = 0; // офсет всегда ноль

    m_header.crc8_header = 0; // crc заголовка считать не надо
    m_header.data_crc = 0;    // crc данных считать не надо

    return BodyAdd((const uint8_t *)&m_header, sizeof(m_header));
}
