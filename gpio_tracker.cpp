#include "gpio_tracker.h"

GpioTracker::GpioTracker(int gpio_n, bool dir, QObject *parent)
   : QObject(parent)
{
    gpio_number = gpio_n;
    gpio_direction = TGpioDirection(dir);
}

GpioTracker::~GpioTracker()
{
    file->close();
}

bool GpioTracker::Init()
{
    // echo 26 > /sys/class/gpio/export
    // echo out > /sys/class/gpio/gpio26/direction
    // echo in > /sys/class/gpio/gpio26/direction
    // cat /sys/class/gpio/gpio26/value
    // 0
    bool res = true;

    // Ask the kernel to export control of a GPIO to userspace
    // echo 26 > /sys/class/gpio/export
    QFile exportFile("/sys/class/gpio/export");
    if(!exportFile.open(QIODevice::WriteOnly))
    {
        emit consolePutData("GPIO export file open failed\n", 2);
        res = false;
    }
    QString gpioNumber = QString::number(gpio_number);
    if(exportFile.write(gpioNumber.toUtf8()) == -1)
    {
        emit consolePutData("GPIO export file write failed\n", 2);
        res = false;
    }
    exportFile.close();

    QThread::sleep(1);

    // Set gpio direction
    // echo in > /sys/class/gpio/gpio26/direction
    // echo out > /sys/class/gpio/gpio26/direction
    QFile directionFile(QString("/sys/class/gpio/gpio%1/direction").arg(gpio_number));
    if(!directionFile.open(QIODevice::WriteOnly))
    {
        emit consolePutData("GPIO direction file open failed\n", 2);
        res = false;
    }
    switch(gpio_direction)
    {
        case DirectionIn:
            if(directionFile.write("in") == -1)
            {
                emit consolePutData("GPIO direction file write failed\n", 2);
                res = false;
            }
            break;
        case DirectionOut:
            if(directionFile.write("out") == -1)
            {
                emit consolePutData("GPIO direction file write failed\n", 2);
                res = false;
            }
            break;
    }
    directionFile.close();

    // Setup file to read\write gpio value
    file.reset(new QFile());
    auto pathname = QStringLiteral("/sys/class/gpio/gpio%1/value").arg(gpio_number);
    file->setFileName(pathname);

    // Check if GPIO value file exists
    if(!file->exists())
    {
        emit consolePutData("Error GPIO " + pathname + " file does not exist\n", 2);
        res = false;
        return res;
    }

    if(!file->open(QIODevice::ReadOnly))
    {
        emit consolePutData("Error GPIO value file open failed", 2);
        res = false;
        return res;
    }

    emit consolePutData(QString("GPIO %1 read: ").arg(gpio_number) + file->peek(1) + "\n", 2);
    file->close();

    return res;
}

bool GpioTracker::readValue()
{
    if(file == nullptr)
        return false;

    if(file->open(QIODevice::ReadOnly))
    {
        QByteArray val = file->peek(1);

        if(val.size())
        {
            if(val.at(0) == '1')
            {
                file->close();
                return true;
            }
        }
    }

    file->close();
    return false;
}

bool GpioTracker::writeValue(quint8 value)
{
    if(file == nullptr)
        return false;

    if(file->open(QIODevice::ReadWrite))
    {
        if(value)
            file->write("1");
        else
            file->write("0");
    }

    emit consolePutData(QString("GPIO %1 write: %2\n").arg(gpio_number).arg(value), 2);

    file->close();
    return false;
}
