#ifndef MOD_TRANSMITTER_THREAD_H
#define MOD_TRANSMITTER_THREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QElapsedTimer>
#include <QTimer>
#include <message_box.h>

class ModTransmitterThread : public QThread
{
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);
    void postDataToStm32H7(const uint8_t *p_data, const int length);
    void startAnswerTimeoutTimer(int timeout_ms);
    void stopAnswerTimeoutTimer();

public slots:
    void timeoutAnswer();

public:
    explicit ModTransmitterThread(QObject *parent = nullptr);
    ~ModTransmitterThread();

    bool ModAnswerReceived(const uint8_t *p_data, int length);
    void ModStartTransmitPhaseGain();

private slots:
    //void timeoutAnswer();

private:
    void run() override;
    //void FreqEstimateForSweep();
    //void Sweep();
    //bool ConvertToDouble(uint8_t *p_data_in, uint32_t length_in, double *p_data_out, double *p_length);

    bool m_quit = false;

    //double LengthSin = 0.f;
    //double LengthSweep = 0.f;

    //QElapsedTimer peformance_timer;     // timer for QAM decoder performance measurements
    //QElapsedTimer response_timeout_timer;

    message_header message;
    uint8_t message_box_buffer_mod[1024];
    uint8_t n_channel = 0;

    uint32_t n_elements = 2048;

    typedef enum
    {
        IDLE = 0,
        TRANSMIT_PHASE = 1,
        TRANSMIT_GAIN = 2
    } TState;

    TState State = IDLE;

    bool retry = false;
    uint32_t n_attempts = 0;

    const uint32_t n_MaxAttempts = 10;

    const uint32_t timeoutAnswer_ms = 200;
};

#endif // MOD_TRANSMITTER_THREAD_H
