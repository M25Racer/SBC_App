#include <QtGlobal>
//#include "StdAfx.h"
#include "IndigoBaseTypesV1.h"
#include "IndigoBaseProtocolBaseParserV1.h"
#include "crc16.h"
#include "crc8.h"

uint32_t IndigoBaseProtocolBaseParserV1::GetLastError() const
{
    return m_last_error;
}

void IndigoBaseProtocolBaseParserV1::SetNotify(IndigoBaseProtocolParserNotify notify, const void *param)
{
    m_notify = notify;
    m_notify_param = param;
}

bool IndigoBaseProtocolBaseParserV1::ValidateHeader(const TransportProtocolHeaderV1 *header)
{
    if (header->version == TRANSPORT_LEVEL_VERSION)
    {
        if (header->header_length != sizeof(TransportProtocolHeaderV1))
            return false;

        // проверим, размер буфера меньше тела сообщения, должен быть не больше размера буфера
        if (header->data_length > TRANSPORT_MAX_SIZE_BODY)
            return false;

        // check crc8
        if (!check_crc8((const uint8_t *)header, offsetof(TransportProtocolHeaderV1, crc8_header), header->crc8_header))
            return false;
        return true;
    }
    // неизвестная версия
    return false;
}

bool IndigoBaseProtocolBaseParserV1::ValidateBody(const TransportProtocolHeaderV1 *header, const uint8_t *body)
{
    if (header->version == TRANSPORT_LEVEL_VERSION)
    {
        if (!check_crc16(body, header->data_length, header->data_crc))
            return false;
        return true;
    }
    // неизвестная версия
    return false;
}
