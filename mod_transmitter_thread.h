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

    void ModAnswerDataReceived();

private slots:
    //void timeoutAnswer();

private:
    void run() override;

    bool m_quit = false;

    message_header message;
    uint8_t message_box_buffer_mod[1024];
    uint8_t n_channel = 0;

    uint32_t n_elements = 2048;

    typedef enum
    {
        IDLE = 0,
        TX_PHASE_TABLE_1 = 1,
        TX_GAIN_TABLE_2 = 2,
        TX_SHIFT_3 = 3
    } TState;

    TState State = IDLE;

    bool retry = false;
    uint32_t n_attempts = 0;

    const uint32_t n_MaxAttempts = 10;
    const uint32_t timeoutAnswer_ms = 500;

    bool hs_data_received = false;      // If true, indicates that at least some 'HS' data was received from usb STM32 H7
};

#endif // MOD_TRANSMITTER_THREAD_H
