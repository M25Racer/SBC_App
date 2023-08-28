#ifndef BLACKBOX_H
#define BLACKBOX_H

#include <QCoreApplication>
#include <QTextStream>
#include <QScopedPointer>
#include <QDir>
#include "atomic_vars.h"

class BlackBox : public QObject
{
public:
    Q_OBJECT

public:
    explicit BlackBox(QObject *parent = nullptr);
    ~BlackBox();

    void Init();
    bool Save();
    void Read();

protected:

    QTextStream out;
    bool n_blackbox = false;        // current blackbox file to write to

    // Limits for 'f0' carrier frequency parameter, when loading from BlackBox files
    const double n_LimitLow_f0 = (35045 - 100);
    const double n_LimitHigh_f0 = (35045 + 100);

private:

};

#endif // BLACKBOX_H
