#ifndef MEDIAN_WINDOW_FILTER_H
#define MEDIAN_WINDOW_FILTER_H

#include <QCoreApplication>

class MedianWindowFilter : public QObject
{
public:
    Q_OBJECT

public:
    explicit MedianWindowFilter(uint16_t FltWindowSize = 10, QObject *parent = nullptr);
    ~MedianWindowFilter();

    bool Reset();
    void AddElement(double val);
    bool IsFilled();
    double Filter();

protected:
    uint16_t n_elements = 0;
    double *FilterArray = nullptr;
    QList<double> sortList;
    double median = 0;

    uint32_t in = 0;
    bool m_isFilterFilled = false;

private:

};

#endif // MEDIAN_WINDOW_FILTER_H
