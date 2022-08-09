#include "synchro_watcher.h"
#include <QMutex>
#include <QElapsedTimer>

static QMutex mutex;
static bool m_syncIsActive = false;
static uint32_t m_suspendedTime = 0;

static QElapsedTimer synchro_measure_timer;

bool syncIsSuspendedTimeInProgress()
{
    bool s;

    mutex.lock();
    s = m_syncIsActive;
    mutex.unlock();

    return s;
}

void syncSetSuspendedTimeInProgress(bool s, uint16_t suspended_time_ms)
{
    mutex.lock();
    m_syncIsActive = s;

    // If suspended time is in progress start timer
    if(s)
        synchro_measure_timer.start();

    m_suspendedTime = suspended_time_ms;
    mutex.unlock();
}

uint16_t syncGetTime()
{
    mutex.lock();

    // Calculate suspended time left since USB_CMD_SYNCHRO_START
    qint64 elapsed = synchro_measure_timer.elapsed();
    uint16_t time_left = m_suspendedTime > elapsed ? m_suspendedTime - elapsed : 0;

    mutex.unlock();

    return time_left;
}
