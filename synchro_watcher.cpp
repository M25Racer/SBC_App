#include <QtGlobal>
#include <QMutex>

static QMutex mutex;
static bool m_syncIsActive = false;
static uint32_t m_suspendedTime = 0;

bool syncIsSuspendedTimeInProgress()
{
    bool s;

    mutex.lock();
    s = m_syncIsActive;
    mutex.unlock();

    return s;
}

void syncSetSuspendedTimeInProgress(bool s)
{
    mutex.lock();
    m_syncIsActive = s;
    mutex.unlock();
}
