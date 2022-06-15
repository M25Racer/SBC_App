#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QCoreApplication>
#include "usb_global.h"

namespace RingBufferNS
{
    const uint32_t N_ELEMENTS = (10);
    //const uint32_t DATA_SIZE = (10*1024*1024);//(256*1024);
}

using namespace RingBufferNS;

class RingBuffer : public QObject
{
public:
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);

public:
    bool Append(const uint8_t *p_data_in, const uint32_t data_length);
    bool Get(uint8_t *p_data_out, uint32_t *p_length);
    bool GetDouble(double *p_data_out, double *p_length);
    void Clear();
    void Init(){ Clear(); };
    bool DataAvailable();
    void SetActive(bool s);

protected:

private:
    // Access to different elements can be done in a thread safe way
    uint32_t DataLength[N_ELEMENTS];

    // Access to this variables should be protected with mutex
    uint32_t tail = 0;
    uint32_t head = 0;

    bool m_isActive = true;
};

#endif // RINGBUFFER_H
