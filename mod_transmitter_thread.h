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
    void sendCommandToSTM32(quint8 command, const quint8 *p_data, quint32 data_size);
    void qamDecoderReset();

public slots:
    void timeoutAnswer();

public:
    explicit ModTransmitterThread(QObject *parent = nullptr);
    ~ModTransmitterThread();

    bool ModAnswerReceived(const uint8_t *p_data, int length);
    void ModStartTransmitPhaseGain();

    void ModAnswerDataReceived();
    void calculatePredistortionTablesStart();

private:
    void run() override;
    void transmitPredistortionTables();
    void calculatePredistortionTablesStop();

    bool m_quit = false;

    message_header message;
    uint8_t message_box_buffer_mod[1024];
    uint8_t n_channel = 0;

    uint32_t n_elements = 2048;

    typedef enum
    {
        IDLE = 0,
        AUTOCFG_START = 1,
        SIN35KHZ_MOD_COMMANDS_FOR_AGC = 2,
        ADC_START_FOR_SIN600 = 3,
        SIN600_MOD_COMMAND = 4,
        FREQ_ESTIMATE_FUNC_WAIT = 5,
        AGC_START_FOR_SWEEP = 6,
        SWEEP_MOD_COMMANDS_FOR_AGC = 7,
        ADC_START_FOR_SWEEP = 8,
        SWEEP_MOD_COMMAND = 9,
        SWEEP_FUNC_WAIT = 10,
        AGC_START_FOR_MOD_STAT = 11,
        STAT_MOD_COMMANDS_FOR_AGC = 12,
        START_TX_PREDISTORTION_TABLES_TO_MOD = 13,
        TX_PREDISTORTION_TABLES_TO_MOD = 14,
        AGC_START_FOR_MOD_STAT2 = 15,
        STAT_MOD_COMMANDS_FOR_AGC2 = 16,
        AUTOCFG_COMPLETE = 17
    } TState;

    TState State = IDLE;
    TState StatePrev = IDLE;

    typedef enum
    {
        TX_IDLE = 0,
        TX_START = 1,
        TX_PHASE_TABLE = 2,
        TX_GAIN_TABLE = 3,
        TX_SHIFT_CRC = 4,
        TX_FINISHED = 5
    } TPredistTxState;

    TPredistTxState StatePredistTx = TX_IDLE;

    bool retry = false;
    uint32_t n_attempts = 0;
    uint32_t n_attempts_high_level = 0;

    const uint32_t n_MaxAttempts = 5;
    const uint32_t n_MaxAttemptsHighLevel = 3;
    const uint32_t timeoutAnswer_ms = 600;

    const uint32_t timeoutAgcSin35kHzCommands_ms = 500;     // timeout between SIN 35 kHz transfers for AGC
    const uint32_t timeoutAgcSweepCommands_ms = 1000;       // timeout between Sweep transfers for AGC
    const uint32_t timeoutModStatusCommands_ms = 300;       // timeout between MOD GET STATUS transfers for AGC

    const uint32_t n_MaxSin35kHzCommands = 100;             // max number of SIN 35 kHz transfers for AGC
    const uint32_t n_MaxSweepCommands = 100;                // max number of Sweep transfers for AGC
    const uint32_t n_MaxModStatusCommands = 100;            // max number of MOD STATUS transfers for AGC

    bool hs_data_received = false;      // If true, indicates that at least some 'HS' data was received from usb STM32 H7
};

#endif // MOD_TRANSMITTER_THREAD_H
