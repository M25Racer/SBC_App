#pragma once

#if !defined (_MSC_VER)
#include "stdint.h"
#endif

#include "TransportProtocolHeader.h"
#include "CommandHeaderHeader.h"
#include "IndigoBaseProtocolFrameBuilderV1.h"
#include "IndigoProtocolHelpClasses.h"
#include "IndigoBaseProtocolBuilderV1.h"


class IndigoProtocolUsbBuilderV1
{
public:
    IndigoProtocolUsbBuilderV1();


    uint32_t GetLastError() const;

    void  SetAddress(const ToolAddress *address_src, const ToolAddress *address_dest);
    void  SetNotify(IndigoBaseProtocolBuilderNotify notify, const void *param);

    void  Initialize();
    void  AddOffsets(CmdRepeatOffset *offsets, uint32_t count);

    void  SetDataLength(uint32_t data_length);
    bool  BodyAdd(const uint8_t *body, uint32_t size);
    bool  BodyEnd();


private:
    enum Constants
    {
        LAST_ERROR_SUCCESS = 0,
        LAST_ERROR_NOT_ENOUGH_MEMORY,
		LAST_ERROR_INVALID_SIZE,
        LAST_ERROR_NULL_POINTER,
        LAST_ERROR_NOTIFY,
        LAST_ERROR_MAX_OFFSETS,
    };

    uint32_t m_last_err;
    TransportProtocolHeaderV1 m_header;
    IndigoBaseProtocolBuilderNotify m_notify;
    const void *m_param;
    uint8_t m_packet_num;


private:
    bool CreateHeader();
};
