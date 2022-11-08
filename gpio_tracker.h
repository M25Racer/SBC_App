#ifndef GPIO_TRACKER_H
#define GPIO_TRACKER_H

#include <QCoreApplication>
#include <QFile>
#include <QThread>

class GpioTracker : public QObject
{
    Q_OBJECT

public:
    explicit GpioTracker(int gpio_n, bool dir, QObject *parent = nullptr);
    ~GpioTracker();

    bool Init();

signals:
    void consolePutData(const QString &data, quint8 priority);

public slots:
    bool readValue();
    bool writeValue(quint8 value);

private:

    typedef enum : uint8_t
    {
        DirectionIn = 0,
        DirectionOut = 1
    } TGpioDirection;

    int gpio_number = 0;
    TGpioDirection gpio_direction = DirectionIn;

    QScopedPointer<QFile> file;
};

#endif // GPIO_TRACKER_H
