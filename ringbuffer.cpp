#include <QCoreApplication>
//#include <QDebug>
#include <ringbuffer.h>

/*
tail = head ? - no elements in ring buffer
head points to first free element

 tail  head
 O     O     O     O ...
*/

void RingBuffer::Clear()
{
    //todo protect
    head = tail = 0;
    memset(DataLength, 0, sizeof(DataLength));
    //todo protect
}

bool RingBuffer::DataAvailable()
{
    //todo protect
    if(tail == head)
        return false;
    //todo protect

    return true;
}

bool RingBuffer::Append(const uint8_t *p_data_in, const uint32_t data_length)
{
    // Check data pointer
    if(p_data_in == nullptr)
    {
        emit consolePutData("Error ring buffer append: data pointer = null", 1);
        return false;
    }

    // Check length
    if(data_length > DATA_SIZE)
    {
        emit consolePutData(QString("Error ring buffer append: length %1 is too big").arg(data_length), 1);
        return false;
    }

    // Move 'head'
//todo protect
    uint32_t h = head;
    uint32_t t = tail;
//todo protect

    if(++h == N_ELEMENTS)
        h = 0;

    // Check if we catch a tail?
    if(h == t)
    {
        emit consolePutData(QString("Error ring buffer append: head = tail"), 1);
        return false;
    }

    // Copy data
    for(uint32_t i = 0; i < data_length; ++i)
    {
        RingBuf[h][i] = p_data_in[i];
    }

    // Copy data length
    DataLength[h] = data_length;

//todo protect
    head = h;
//todo protect
    return true;
}

bool RingBuffer::Get(uint8_t *p_data_out, uint32_t *p_length)
{
    // Check pointers
    if(p_data_out == nullptr || p_length == nullptr)
    {
        emit consolePutData("Error ring buffer get: data pointer or length pointer = null", 1);
        return false;
    }

//todo protect
    uint32_t h = head;
    uint32_t t = tail;
//todo protect

    // Check if there is no new elements in ring buffer
    if(t == h)
    {
        //emit consolePutData(QString("Error ring buffer get: no elements in ring buffer"), 1);
        return false;
    }

    // Move 'tail'
    if(++t == N_ELEMENTS)
        t = 0;

    // Copy data
    *p_length = DataLength[t];

    for(uint32_t i = 0; i < DataLength[t]; ++i)
    {
        p_data_out[i] = RingBuf[t][i];
    }

//todo protect
    tail = t;
//todo protect

    return true;
}
