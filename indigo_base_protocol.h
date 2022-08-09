#ifndef INDIGO_BASE_PROTOCOL_H
#define INDIGO_BASE_PROTOCOL_H

#include <QtGlobal>
#include "IndigoProtocol/IndigoBaseTypes.h"
#include "IndigoProtocol/V1/IndigoBaseProtocolBuilderV1.h"
#include "IndigoProtocol/V1/IndigoProtocolCommandBuilder.h"
#include "IndigoProtocol/V1/CommandBodyHeader.h"
#include "IndigoProtocol/V1/CommandHeaderHeader.h"

#include "IndigoProtocol/constants.h"

void command_sync_wait_creator(uint16_t time);

#endif // INDIGO_BASE_PROTOCOL_H
