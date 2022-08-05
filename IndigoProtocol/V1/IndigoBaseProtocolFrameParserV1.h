#pragma once

#if !defined (_MSC_VER)
#include "stdint.h"
#include "stdio.h"
#endif

#include "TransportProtocolHeader.h"
#include "IndigoBaseProtocolBaseParserV1.h"

class IndigoBaseProtocolFrameParserV1 : public IndigoBaseProtocolBaseParserV1
{
public:
    IndigoBaseProtocolFrameParserV1();
    ~IndigoBaseProtocolFrameParserV1();

    void     Initalize();
    bool     Parse(const uint8_t *data, uint32_t size);


private:
    bool     ParseHeader(TransportProtocolHeaderV1 **header, const uint8_t **data, uint32_t *size);
    bool     ParseBody(const TransportProtocolHeaderV1 *header, const uint8_t **data, uint32_t *size);
};