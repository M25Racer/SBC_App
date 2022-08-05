#pragma once

#if !defined (_MSC_VER)
#include "stdint.h"
#endif

#include "TransportProtocolHeader.h"
#include "CommandHeaderHeader.h"
#include "IndigoBaseProtocolFrameBuilderV1.h"
#include "IndigoProtocolHelpClasses.h"

typedef bool(*IndigoBaseProtocolBuilderNotify) (const void *param, const uint8_t *buffer, uint32_t size);

class IndigoBaseProtocolBuilderV1
{
public:
    IndigoBaseProtocolBuilderV1();
    ~IndigoBaseProtocolBuilderV1();

    uint32_t GetLastError() const;

    void  SetAddress(const ToolAddress *address_src, const ToolAddress *address_dest);
    void  SetNotify(IndigoBaseProtocolBuilderNotify notify, const void *param);

    void  Initialize();
    void  AddOffsets(CmdRepeatOffset *offsets, uint32_t count);

    bool  BodyAdd(const uint8_t *body, uint32_t size);
    bool  BodyEnd();


private:
    enum constants
    {
        LAST_ERROR_SUCCESS = 0,
        LAST_ERROR_NOT_ENOUGH_MEMORY,
		LAST_ERROR_INVALID_SIZE,
        LAST_ERROR_NULL_POINTER,
        LAST_ERROR_NOTIFY,
        LAST_ERROR_MAX_OFFSETS,
    };

private:
    IndigoBaseProtocolFrameBuilderV1 m_frag_builder;
    uint32_t  m_last_error = LAST_ERROR_SUCCESS;
    
    IndigoBaseProtocolBuilderNotify m_transmit_notify = NULL;
    const void *m_param = NULL;

    bool m_one_frame = true;     // один фрейм в транспортном пакете или нет
    uint32_t  m_pos = 0;         // текущая позиция данных (сколько данных уже получено)
    uint32_t  m_offs_pos = 0;    // позиция офсета с которой формируем фрагменты

    IndigoProtocolMemPack m_buf_in;
    IndigoProtocolMemPack m_buf_out;
        
    IndigoProtocolOffsetStorage m_offsets;


private:
    bool Add(const uint8_t *body, uint32_t size);
    bool WriteFragment(uint32_t flags);

    bool TransmitFragment(const TransportProtocolHeaderV1 *header, const uint8_t *body);


private:
    static bool TransmitNotify(const void *param, const TransportProtocolHeaderV1 *header, const uint8_t *body);
};
