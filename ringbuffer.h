#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <QCoreApplication>

class RingBuffer : public QObject
{
public:
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);

public:
    bool Append(const uint8_t *p_data_in, const uint32_t data_length);
    bool Get(uint8_t *p_data_out, uint32_t *p_length);
    void Clear();
    void Init(){ Clear(); };
    bool DataAvailable();

    static const uint32_t N_ELEMENTS = 10;

protected:

private:
    static const uint32_t DATA_SIZE = (256*1024);

    uint8_t RingBuf[N_ELEMENTS][DATA_SIZE];
    uint32_t DataLength[N_ELEMENTS];

    uint32_t tail = 0;
    uint32_t head = 0;
};

#endif // RINGBUFFER_H
