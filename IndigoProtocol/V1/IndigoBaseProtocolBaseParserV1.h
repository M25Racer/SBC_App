#pragma once

#if !defined (_MSC_VER)
#include "stdint.h"
#include "stdio.h"
#endif
#include "TransportProtocolHeader.h"

typedef bool(*IndigoBaseProtocolParserNotify)(const void *param, const TransportProtocolHeaderV1 *header, const uint8_t *body);

class IndigoBaseProtocolBaseParserV1
{
public:
    enum constants
    {
        LAST_ERROR_SUCCESS = 0,
        LAST_ERROR_NOT_ENOUGH_MEMORY,
        LAST_ERROR_NOT_ENOUGH_BUFFER,
        LAST_ERROR_CHECK_CRC_BODY,
        LAST_ERROR_INCORRECT_HEADER,
        LAST_ERROR_NULL_POINTER,
        LAST_ERROR_NOTIFY,
    };

protected:
    uint32_t  m_last_error = LAST_ERROR_SUCCESS;
    IndigoBaseProtocolParserNotify m_notify = NULL;
    const void   *m_notify_param = NULL;


public:
    uint32_t  GetLastError() const;
    void      SetNotify(IndigoBaseProtocolParserNotify notify, const void *param);

protected:
    bool     ValidateHeader(const TransportProtocolHeaderV1 *header);
    bool     ValidateBody(const TransportProtocolHeaderV1 *header, const uint8_t *body);
};