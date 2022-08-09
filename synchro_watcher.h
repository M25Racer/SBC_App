#ifndef SYNCHRO_WATCHER_H
#define SYNCHRO_WATCHER_H

#include <QtGlobal>

bool syncIsSuspendedTimeInProgress();
void syncSetSuspendedTimeInProgress(bool s, uint16_t suspended_time_ms);
uint16_t syncGetTime();

#endif // SYNCHRO_WATCHER_H
