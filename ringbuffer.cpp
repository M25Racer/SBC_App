#include <QCoreApplication>
#include <QMutex>
#include <ringbuffer.h>

/* Extern global variables */
extern QMutex m_mutex;

void RingBuffer::Clear()
{
    m_mutex.lock();
    head = tail = 0;
    memset(DataLength, 0, sizeof(DataLength));
    m_mutex.unlock();
}

bool RingBuffer::DataAvailable()
{
//    m_mutex.lock();
    if(tail == head)
    {
//        m_mutex.unlock();
        return false;
    }

//    m_mutex.unlock();
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
    m_mutex.lock();
    uint32_t h = head;
    uint32_t t = tail;
    m_mutex.unlock();

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

    m_mutex.lock();
    head = h;
    m_mutex.unlock();

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

    m_mutex.lock();
    uint32_t h = head;
    uint32_t t = tail;
    m_mutex.unlock();

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

    m_mutex.lock();
    tail = t;
    m_mutex.unlock();

    return true;
}
