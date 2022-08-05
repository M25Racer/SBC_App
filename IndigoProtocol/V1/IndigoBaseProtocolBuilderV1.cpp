#include <QtGlobal>
//#include "StdAfx.h"
#if !defined (_MSC_VER)
    #include "string.h"
    #include "stddef.h"
    //#include "debug_logger.h"
#endif

#include "../IndigoBaseTypes.h"
#include "IndigoBaseProtocolBuilderV1.h"

IndigoBaseProtocolBuilderV1::IndigoBaseProtocolBuilderV1() : 
 m_buf_in(TRANSPORT_MAX_SIZE_BODY), 
 m_buf_out(TRANSPORT_MAX_SIZE_FRAME)
{
    m_frag_builder.SetNotify(&TransmitNotify, this);
}

IndigoBaseProtocolBuilderV1::~IndigoBaseProtocolBuilderV1()
{
    m_buf_in.Clear();
    m_buf_out.Clear();
}

void IndigoBaseProtocolBuilderV1::SetAddress(const ToolAddress *address_src, const ToolAddress *address_dest)
{
    if (!address_src || !address_dest)
        return;
    m_frag_builder.SetAddress(address_src, address_dest);
}

void IndigoBaseProtocolBuilderV1::SetNotify(IndigoBaseProtocolBuilderNotify notify, const void *param)
{
    m_param = param;
    m_transmit_notify = notify;
}

uint32_t IndigoBaseProtocolBuilderV1::GetLastError() const
{
    return m_last_error;
}

void IndigoBaseProtocolBuilderV1::Initialize()
{
    m_frag_builder.SetIncPacket(true);
    m_buf_in.Clear();
    m_buf_out.Clear();
    m_offsets.Clear();

    m_one_frame = true;
    m_pos = 0;
    m_offs_pos = 0;
}

void IndigoBaseProtocolBuilderV1::AddOffsets(CmdRepeatOffset *offsets, uint32_t count)
{
    m_offsets.Clear();
    m_frag_builder.SetIncPacket(false);

    // проверим сколько офсетов, если больше допустимого не запоминаем их
    if (m_offsets.GetMax() < count)
        return;

    for (uint32_t i = 0; i < count; i++)
    {
        m_offsets.Add(offsets->offset, offsets->length);
        offsets++;
    }
}

bool IndigoBaseProtocolBuilderV1::BodyAdd(const uint8_t *body, uint32_t size)
{
    if (!body)
        return false;

    // если нет данных, выходим без ошибки
    if (size == 0)
        return true;

    // если нет офсетов, отдаем все данные
    if (m_offsets.GetCount() == 0)
    {
        bool res = Add(body, size);
        return res;
    }



    // офсеты есть, анализируем входящий поток

    // формируем диапазон входных данных
    IndigoProtocolSet range(m_pos, m_pos + size - 1);

    // до тех пор пока диапазон не пустой
    while (!range.IsEmpty())
    {
        // ищем наименьший офсет, который перекрывается с входным диапазоном
        IndigoProtocolSet *offs = m_offsets.GetMinIntersection(&range);
        // таких офсетов нет, значит нечего добавлять, выходим
        if (!offs)
            break;
        
        // ищем пересечение входных данных и найденого офсета  
        IndigoProtocolSet inter_rng = offs->IntersectionOfSets(&range);
        // если левая граница совпадает, значит первый раз обрабатывается найденный офсет
        if (inter_rng.EqualLeft(offs))
            m_offs_pos = offs->GetLeft(); // запомним стартовое смещение

        // рассчитаем относительную (по входным данным) позицию
        uint32_t pos = inter_rng.GetLeft() - m_pos;
        if (!Add(body + pos, inter_rng.GetLength()))
            return false;

        // если совпадает правая граница пересечения и офсета
        // значит текущий офсет полностью обработан, 
        // если за текущим офсетом есть еще офсеты, принудительно формируем последний фрагмент этого офсета
        if (inter_rng.EqualRight(offs) && m_offsets.GetRemaind() > 0)
        {
            if (!WriteFragment(TRANSPORT_FLAG_FRAGMENTATION))
                return false;
        }
        // сместим левую границу, т.к. все что слева пересечения уже обработали
        range.SetLeft(inter_rng.GetRight() + 1);
    }

    m_pos += size;
    return true;
}

bool IndigoBaseProtocolBuilderV1::BodyEnd()
{
    uint32_t flags = m_one_frame ? 0 : TRANSPORT_FLAG_FRAGMENTATION_LAST; 
    if (!WriteFragment(flags))
        return false;

    return true;
}

bool IndigoBaseProtocolBuilderV1::Add(const uint8_t *body, uint32_t size)
{
    do 
    {
        uint32_t remaind = 0;
        if (!m_buf_in.AddPartially(body, size, &remaind))
            return false;

        uint32_t add_bytes = size - remaind;
        body += add_bytes;
        size -= add_bytes;

        // запишем фрагмент пакета, только в том случае, когда буфер заполнен и остались еще данные
        // в противном случае мы не знаем, это последний пакет или нет
        if (m_buf_in.IsFull() && size)
        {
            if (!WriteFragment(TRANSPORT_FLAG_FRAGMENTATION))
                return false;
        }

    } while (size);

    return true;
}

bool IndigoBaseProtocolBuilderV1::WriteFragment(uint32_t flags)
{
    if (m_buf_in.GetLength() == 0)
        return true;

    if (!m_frag_builder.Create(m_buf_in.GetData(), flags, m_buf_in.GetLength(), m_offs_pos))
    {
        m_last_error = m_frag_builder.GetLastError();
        return false;
    }

    // сбросим флаг - одного фрагмента в пакете
    m_one_frame = false;

    m_offs_pos += m_buf_in.GetLength();
    m_buf_in.Clear();
    return true;
}

bool IndigoBaseProtocolBuilderV1::TransmitFragment(const TransportProtocolHeaderV1 *header, const uint8_t *body)
{
    m_buf_out.Clear();


    uint32_t full_size = header->header_length + header->data_length;
    if (full_size > TRANSPORT_MAX_SIZE_FRAME)
    {
    	m_last_error = LAST_ERROR_INVALID_SIZE;
        return false;
    }

    if(full_size > m_buf_out.GetMaxSize())
    {
        m_last_error = LAST_ERROR_NOT_ENOUGH_MEMORY;
        return false;
    }
     
    if (!m_buf_out.Add(header, header->header_length))
    {
        m_last_error = LAST_ERROR_NOT_ENOUGH_MEMORY;
        return false;
    }

    if (!m_buf_out.Add(body, header->data_length))
    {
        m_last_error = LAST_ERROR_NOT_ENOUGH_MEMORY;
        return false;
    }

    if (full_size != m_buf_out.GetLength())
    {
    	m_last_error = LAST_ERROR_INVALID_SIZE;
        return false;
    }

    if (m_transmit_notify)
    {
        if (!m_transmit_notify(m_param, m_buf_out.GetData(), m_buf_out.GetLength()))
        {
        	m_last_error = LAST_ERROR_NOTIFY;
            return false;
        }
    }
    return true;
}

bool IndigoBaseProtocolBuilderV1::TransmitNotify(const void *param, const TransportProtocolHeaderV1 *header, const uint8_t *body)
{
    if (!param)
        return false;

    IndigoBaseProtocolBuilderV1 *self = (IndigoBaseProtocolBuilderV1 *)param;
    if (!self->TransmitFragment(header, body))
    	return false;
    return true;
}
