#include <QtGlobal>
//#include "StdAfx.h"
#if !defined (_MSC_VER)
#endif
#include "IndigoProtocolHelpClasses.h"


IndigoProtocolSet::IndigoProtocolSet() : 
m_left(0),
m_right(0)
{
}

IndigoProtocolSet::IndigoProtocolSet(uint32_t left, uint32_t right) :
m_left(left), 
m_right(right)
{
}

IndigoProtocolSet::~IndigoProtocolSet()
{
    m_left = 0;
    m_right = 0;
}

void IndigoProtocolSet::Set(uint32_t left, uint32_t right)
{
    m_left = left;
    m_right = right;
}

void IndigoProtocolSet::SetLeft(uint32_t left)
{
    m_left = left;
}

void IndigoProtocolSet::SetRight(uint32_t right)
{
    m_right = right;
}

uint32_t IndigoProtocolSet::GetLeft() const
{
    return m_left;
}

uint32_t IndigoProtocolSet::GetRight() const
{
    return m_right;
}

uint32_t IndigoProtocolSet::GetLength() const
{
    uint32_t len = m_right - m_left + 1;
    return len;
}

bool IndigoProtocolSet::IsEmpty() const
{
    if (m_left > m_right)
        return true;
    return false;
}

bool IndigoProtocolSet::Less(const IndigoProtocolSet *set) const
{
    if (this->m_right < set->m_left)
        return true;
    return false;
}

bool IndigoProtocolSet::Great(const IndigoProtocolSet *set) const
{
    if (this->m_left > set->m_right)
        return true;
    return false;
}

bool IndigoProtocolSet::Equal(const IndigoProtocolSet *set) const
{
    if (this->m_left == set->m_left && 
        this->m_right == set->m_right)
        return true;
    return false;
}

bool IndigoProtocolSet::EqualLeft(const IndigoProtocolSet *set) const
{
    if (this->m_left == set->m_left)
        return true;
    return false;
}

bool IndigoProtocolSet::EqualRight(const IndigoProtocolSet *set) const
{
    if (this->m_right == set->m_right)
        return true;
    return false;
}

IndigoProtocolSet IndigoProtocolSet::IntersectionOfSets(const IndigoProtocolSet *set) const
{
    IndigoProtocolSet ret;
    uint32_t left = max(this->m_left, set->m_left);
    uint32_t right = min(this->m_right, set->m_right);
    ret.Set(left, right);
    return ret;
}

/*
*/
IndigoProtocolMemPack::IndigoProtocolMemPack(uint32_t max_size) : 
m_length(0),
m_max_size(max_size)
{
#if defined(DEBUG) || defined(_DEBUG)
    memset(m_buffer, 0, sizeof(m_buffer));
#endif

    if (m_max_size > sizeof(m_buffer))
        m_max_size = sizeof(m_buffer);
}

bool IndigoProtocolMemPack::Add(const void *data, uint32_t size)
{
    if (GetRemaind() < size)
        return false;

    if (!memcpy(m_buffer + m_length, data, size))
        return false;

    m_length += size;
    return true;
}

bool IndigoProtocolMemPack::AddPartially(const void *data, uint32_t size, uint32_t *remaind)
{
    *remaind = 0;
    if (GetRemaind() < size)
    {
        *remaind = size - GetRemaind();
        size -= *remaind;
    }

    if (!memcpy(m_buffer + m_length, data, size))
        return false;

    m_length += size;
    return true;
}

void IndigoProtocolMemPack::Clear()
{
#if defined(DEBUG) || defined(_DEBUG)
    memset(m_buffer, 0, sizeof(m_buffer));
#endif 
    m_length = 0;
}

const uint8_t * IndigoProtocolMemPack::GetData() const
{
    return &m_buffer[0];
}

uint32_t IndigoProtocolMemPack::GetLength() const
{
    return m_length;
}

uint32_t IndigoProtocolMemPack::GetMaxSize() const
{
    return m_max_size;
}

bool IndigoProtocolMemPack::IsFull() const
{
    if (m_length >= m_max_size)
        return true;
    return false;
}

uint32_t IndigoProtocolMemPack::GetRemaind() const
{
    uint32_t remaind = m_max_size - m_length;
    return remaind;
}

/*
*/
IndigoProtocolOffsetStorage::IndigoProtocolOffsetStorage() :
m_count(0), 
m_pos(0)
{
}

void IndigoProtocolOffsetStorage::Clear()
{
    m_count = 0;
    m_pos = 0;
}

uint32_t IndigoProtocolOffsetStorage::GetMax() const
{
    return MAX_OFFSETS;
}

bool IndigoProtocolOffsetStorage::Add(uint32_t offset, uint32_t length)
{
    if (m_count >= sizeof(m_offsets))
        return false;
    if (length == 0)
        return false;

    IndigoProtocolSet *item = &m_offsets[m_count];
    if (length == END_OF_DATA)
        item->Set(offset, END_OF_DATA);
    else
        item->Set(offset, offset + length - 1);
    m_count++;
    return true;
}

uint32_t IndigoProtocolOffsetStorage::GetCount() const
{
    return m_count;
}

uint32_t IndigoProtocolOffsetStorage::GetRemaind() const
{
    if (m_pos >= m_count)
        return 0;

    uint32_t remaind = m_count - m_pos - 1;
    return remaind;
}

IndigoProtocolSet *IndigoProtocolOffsetStorage::GetMinIntersection(const IndigoProtocolSet *src_offs)
{
    for (uint32_t i = m_pos; i < m_count; i++)
    {
        IndigoProtocolSet *offs = &m_offsets[i];
        if (offs->Less(src_offs)) // офсет меньше чем искомый, поднимаем позицию и ищем далее
        {
            m_pos++;
            continue;
        }
        else if (offs->Great(src_offs))  // офсет больше чем искомый, смысла искать дальше нет, ничего не нашли выходим
            return NULL;
        else  // нашли пересечение
            return offs;
    }
    return NULL;
}
