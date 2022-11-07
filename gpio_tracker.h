#ifndef GPIO_TRACKER_H
#define GPIO_TRACKER_H

#include <QCoreApplication>
#include <QFile>
#include <QThread>

class GpioTracker : public QObject
{
    Q_OBJECT

public:
    explicit GpioTracker(int gpio_n, QObject *parent = nullptr);
    ~GpioTracker();

    bool Init();

signals:
    void consolePutData(const QString &data, quint8 priority);

public slots:
    bool readValue();

private:
    int gpio_number = 0;
    QScopedPointer<QFile> file;
};

#endif // GPIO_TRACKER_H
