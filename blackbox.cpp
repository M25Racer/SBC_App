#include <blackbox.h>
#include <QMutex>
#include <QFileDevice>
#include <QDateTime>

extern QMutex m_freqValMutex;
extern double f0_saved;

extern double f0;   // todo not thread safe

BlackBox::BlackBox(QObject *parent)
   : QObject(parent)
{
}

BlackBox::~BlackBox()
{
}

void BlackBox::Init()
{
    // Check & create folder for 'BlackBox' files
    if(!QDir("BlackBox").exists())
        QDir().mkdir("BlackBox");
}

bool BlackBox::Save()
{
    bool res = false;

    // Get current 'f0' thread-safe way
    double f0_current;
    m_freqValMutex.lock();
    f0_current = f0_saved;
    m_freqValMutex.unlock();

    // Round to .XX precision
    f0_current = (int)(f0_current * 100);
    f0_current /= 100.f;
    //f0_current = (QString::number(f0_current, 'f', 2)).toDouble();

    // Check if 'f0' value is not changed
    static double f0_prev = 0.f;
    if(f0_prev == f0_current)
        return true;

    f0_prev = f0_current;

    if(!n_blackbox)
    {
        QScopedPointer<QFile> m_file1;      // Smart pointer to file
        m_file1.reset(new QFile("BlackBox/BlackBox_1.ini"));

        res = m_file1.data()->open(QFile::ReadWrite | QFile::Text);

        if(!res)
        {
            m_file1->close();
            return false;
        }

        // Clear file contents
        m_file1->resize(0);

        out.setDevice(m_file1.data());
        out << QString("%1\n").arg(f0_current, 0, 'f', 2);
        out.flush();

        m_file1->close();
    }
    else
    {
        QScopedPointer<QFile> m_file2;      // Smart pointer to file
        m_file2.reset(new QFile("BlackBox/BlackBox_2.ini"));

        res = m_file2.data()->open(QFile::ReadWrite | QFile::Text);

        if(!res)
        {
            m_file2->close();
            return false;
        }

        // Clear file contents
        m_file2->resize(0);

        out.setDevice(m_file2.data());
        out << QString("%1\n").arg(f0_current, 0, 'f', 2);
        out.flush();

        m_file2->close();
    }

    n_blackbox = !n_blackbox;
    return true;
}

void BlackBox::Read()
{
    bool res = false;

    // Check & create folder for 'BlackBox' files
    if(!QDir("BlackBox").exists())
    {
        QDir().mkdir("BlackBox");
        return;
    }

    QScopedPointer<QFile> m_file1;      // Smart pointer to file
    QScopedPointer<QFile> m_file2;      // Smart pointer to file
    m_file1.reset(new QFile("BlackBox/BlackBox_1.ini"));
    m_file2.reset(new QFile("BlackBox/BlackBox_2.ini"));

    double val1 = 0.f, val2 = 0.f;
    bool m_use_val1 = false;
    bool m_use_val2 = false;

    // Read file 1
    if(m_file1.data()->exists())
    {
        res = m_file1.data()->open(QFile::ReadOnly | QFile::Text);
        if(!res)
            m_file1->close();

        QTextStream in(m_file1.data());
        QString line = in.readLine();

        val1 = line.toDouble(&res);
        if(res)
        {
            if(val1 > n_LimitLow_f0 && val1 < n_LimitHigh_f0)
                m_use_val1 = true;
        }
    }

    // Read file 2
    if(m_file2.data()->exists())
    {
        res = m_file2.data()->open(QFile::ReadOnly | QFile::Text);
        if(!res)
            m_file2->close();

        QTextStream in(m_file2.data());
        QString line = in.readLine();

        val2 = line.toDouble(&res);
        if(res)
        {
            if(val2 > n_LimitLow_f0 && val2 < n_LimitHigh_f0)
                m_use_val2 = true;
        }
    }

    if(m_use_val1 && m_use_val2)
    {
        if(m_file1.data()->fileTime(QFileDevice::FileModificationTime) > m_file2.data()->fileTime(QFileDevice::FileModificationTime))
            m_use_val2 = false;
        else
            m_use_val1 = false;
    }

    if(m_use_val1)
    {
        f0 = val1;
        n_blackbox = true;
    }
    else if(m_use_val2)
    {
        f0 = val2;
        n_blackbox = false;
    }
    else
    {
        // No data in both files
    }

    if(m_file1->isOpen())
        m_file1->close();

    if(m_file2->isOpen())
        m_file2->close();
}
