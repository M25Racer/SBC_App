#pragma once

#if !defined (_MSC_VER)

#include "stdint.h"
#include <string.h>
#include <stdlib.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#endif



#include "../IndigoBaseTypes.h"
#include "IndigoBaseTypesV1.h"
#include "CommandHeaderHeader.h"

class IndigoProtocolSet
{
public:
    IndigoProtocolSet();
    IndigoProtocolSet(uint32_t left, uint32_t right);
    ~IndigoProtocolSet();
    
    void Set(uint32_t left, uint32_t right);
    void SetLeft(uint32_t left);
    void SetRight(uint32_t right);

    uint32_t GetLeft() const;
    uint32_t GetRight() const;

    uint32_t GetLength() const;

    bool IsEmpty() const;

    bool Less(const IndigoProtocolSet *set) const;
    bool Great(const IndigoProtocolSet *set) const;

    bool Equal(const IndigoProtocolSet *set) const;
    bool EqualLeft(const IndigoProtocolSet *set) const;
    bool EqualRight(const IndigoProtocolSet *set) const;

    // возвращает пересечение двух диапазонов
    IndigoProtocolSet IntersectionOfSets(const IndigoProtocolSet *set) const;


private:
    uint32_t m_left;
    uint32_t m_right;
};


class IndigoProtocolMemPack
{
public:
    IndigoProtocolMemPack(uint32_t max_size);
   
    bool Add(const void *data, uint32_t size);
    bool AddPartially(const void *data, uint32_t size, uint32_t *remaind);
    void Clear();

    const uint8_t *GetData() const;
    uint32_t GetLength() const;
    uint32_t GetMaxSize() const;

    bool IsFull() const;


private:
    uint8_t  m_buffer[TRANSPORT_MAX_SIZE_FRAME];
    uint32_t m_length;
    uint32_t m_max_size;

private:
    uint32_t GetRemaind() const;
};


class IndigoProtocolOffsetStorage
{
public:
    IndigoProtocolOffsetStorage();
    
    void Clear();
    uint32_t GetMax() const;

    bool Add(uint32_t offset, uint32_t length);

    uint32_t GetCount() const;
    uint32_t GetRemaind() const;

    IndigoProtocolSet *GetMinIntersection(const IndigoProtocolSet *set);


private:
    enum constants
    {
        MAX_OFFSETS = INDIGO_PROTOCOL_MAX_OFFSETS,  // максимальное колво офсетов которое может быть 
        END_OF_DATA = CmdRepeatEndData,             // конец данных
    };

    IndigoProtocolSet m_offsets[MAX_OFFSETS];
    uint32_t m_count;     // колво офсетов которое содержит клас
    uint32_t m_pos;       // текущая позиция с которой ищутся офсеты
};
