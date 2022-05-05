#ifndef FREQ_SWEEP_THREAD_H
#define FREQ_SWEEP_THREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QElapsedTimer>

class FreqSweepThread : public QThread
{
    Q_OBJECT

signals:
    void consolePutData(const QString &data, quint8 priority);

public slots:

public:
    explicit FreqSweepThread(QObject *parent = nullptr);
    ~FreqSweepThread();

private:
    void run() override;
    void FreqEstimateForSweep();
    void Sweep();
    bool ConvertToDouble(uint8_t *p_data_in, uint32_t length_in, double *p_data_out, double *p_length);

    bool m_quit = false;

    double LengthSin = 0.f;
    double LengthSweep = 0.f;

    QElapsedTimer peformance_timer;     // timer for QAM decoder performance measurements
};

#endif // FREQ_SWEEP_THREAD_H
