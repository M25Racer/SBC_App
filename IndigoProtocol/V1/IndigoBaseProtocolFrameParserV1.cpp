#include <QtGlobal>
//#include "StdAfx.h"
#include "IndigoBaseProtocolFrameParserV1.h"

IndigoBaseProtocolFrameParserV1::IndigoBaseProtocolFrameParserV1()
{
}

IndigoBaseProtocolFrameParserV1::~IndigoBaseProtocolFrameParserV1()
{
}

void IndigoBaseProtocolFrameParserV1::Initalize()
{
    m_notify = NULL;
    m_notify_param = NULL;
    m_last_error = LAST_ERROR_SUCCESS;
}

bool IndigoBaseProtocolFrameParserV1::Parse(const uint8_t *data, uint32_t size)
{
    m_last_error = LAST_ERROR_SUCCESS;

    TransportProtocolHeaderV1 *header = NULL;
    bool r = ParseHeader(&header, &data, &size);
    if (r)
        r = ParseBody(header, &data, &size);

    return r;
}

bool IndigoBaseProtocolFrameParserV1::ParseHeader(TransportProtocolHeaderV1 **header, const uint8_t **data, uint32_t *size)
{
    // проверим места в буфере хватает?
    if (*size < sizeof(TransportProtocolHeaderV1))
    {
        m_last_error = LAST_ERROR_NOT_ENOUGH_BUFFER;
        return false;
    }

    *header = (TransportProtocolHeaderV1 *)(*data);
    if (!ValidateHeader(*header))
    {
        m_last_error = LAST_ERROR_INCORRECT_HEADER;
        return false;
    }

    if (*size < (*header)->header_length)
    {
        m_last_error = LAST_ERROR_NOT_ENOUGH_BUFFER;
        return false;
    }

    // корректируем на размер считанных данных
    (*data) += (*header)->header_length;
    *size   -= (*header)->header_length; 
    return true; 
}

bool IndigoBaseProtocolFrameParserV1::ParseBody(const TransportProtocolHeaderV1 *header, const uint8_t **data, uint32_t *size)
{
    uint32_t read = header->data_length;
    // проверим размер буфера достаточный?    
    if (read > *size)
    {
        m_last_error = LAST_ERROR_NOT_ENOUGH_BUFFER;
        return false;
    }

    // проверим корректность тела сообщения
    if (!ValidateBody(header, *data))
    {
        m_last_error = LAST_ERROR_CHECK_CRC_BODY;
        return false;
    }

    // уведомим 
    if (m_notify)
    {
        if (!m_notify(m_notify_param, header, *data))
        {
            m_last_error = LAST_ERROR_NOTIFY;
            return false;
        }
    }
   
    // корректируем на прочитанный размер
    *size        -= read;
    (*data)      += read;
    return true;
}
