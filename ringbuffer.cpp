#include <QCoreApplication>
#include <QMutex>
#include <ringbuffer.h>

/* Extern global variables */
static QMutex m_mutex_ring;

/* Private variables */
static uint8_t RingBuf[N_ELEMENTS][USB_MAX_DATA_SIZE];

void RingBuffer::SetActive(bool s)
{
    m_mutex_ring.lock();
    m_isActive = s;

    // Clear
    head = tail = 0;
    memset(DataLength, 0, sizeof(DataLength));
    m_mutex_ring.unlock();
}

void RingBuffer::Clear()
{
    m_mutex_ring.lock();
    head = tail = 0;
    memset(DataLength, 0, sizeof(DataLength));
    m_mutex_ring.unlock();
}

bool RingBuffer::DataAvailable()
{
    m_mutex_ring.lock();
    if(tail == head)
    {
        m_mutex_ring.unlock();
        return false;
    }

    m_mutex_ring.unlock();
    return true;
}

bool RingBuffer::Append(const uint8_t *p_data_in, const uint32_t data_length)
{
    // Check data pointer
    if(p_data_in == nullptr)
    {
        emit consolePutData("Error ring buffer append: data pointer = null", 2);
        return false;
    }

    // Check length
    if(data_length > USB_MAX_DATA_SIZE)
    {
        emit consolePutData(QString("Error ring buffer append: length %1 is too big").arg(data_length), 2);
        return false;
    }

    m_mutex_ring.lock();

    // Check if ring buffer disabled
    if(!m_isActive)
    {
        m_mutex_ring.unlock();
        return false;
    }

    // Move 'head'
    uint32_t h = head;
    uint32_t t = tail;

    if(++h == N_ELEMENTS)
        h = 0;

    // Check if we catch a tail?
    if(h == t)
    {
        emit consolePutData(QString("Error ring buffer append: head = tail"), 2);
        m_mutex_ring.unlock();
        return false;
    }

    // Copy data
    for(uint32_t i = 0; i < data_length; ++i)
    {
        RingBuf[h][i] = p_data_in[i];
    }

    // Copy data length
    DataLength[h] = data_length;

    head = h;

    m_mutex_ring.unlock();
    return true;
}

bool RingBuffer::Get(uint8_t *p_data_out, uint32_t *p_length)
{
    // Check pointers
    if(p_data_out == nullptr || p_length == nullptr)
    {
        emit consolePutData("Error ring buffer get: data pointer or length pointer = null", 2);
        return false;
    }

    m_mutex_ring.lock();
    uint32_t h = head;
    uint32_t t = tail;

    // Check if there is no new elements in ring buffer
    if(t == h)
    {
        //emit consolePutData(QString("Error ring buffer get: no elements in ring buffer"), 2);
        m_mutex_ring.unlock();
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

    tail = t;

    m_mutex_ring.unlock();
    return true;
}

bool RingBuffer::GetDouble(double *p_data_out, double *p_length)
{
    // Check pointers
    if(p_data_out == nullptr || p_length == nullptr)
    {
        emit consolePutData("Error ring buffer get: data pointer or length pointer = null", 2);
        return false;
    }

    m_mutex_ring.lock();
    uint32_t h = head;
    uint32_t t = tail;

    // Check if there is no new elements in ring buffer
    if(t == h)
    {
        //emit consolePutData(QString("Error ring buffer get: no elements in ring buffer"), 2);
        m_mutex_ring.unlock();
        return false;
    }

    // Move 'tail'
    if(++t == N_ELEMENTS)
        t = 0;

    // Copy data
    *p_length = double(DataLength[t]/2);

    uint16_t value;
    uint64_t j = 0;
    for(uint32_t i = 0; i < DataLength[t]; i+=2)
    {
        value = RingBuf[t][i];
        value |= ((uint16_t)RingBuf[t][i+1]) << 8;
        p_data_out[j++] = double((int16_t)value);
    }

    tail = t;

    bool res = false;

    if(!DataLength[t])
        res = false;
    else
        res = true;

    m_mutex_ring.unlock();
    return res;
}
