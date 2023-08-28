#include <median_window_filter.h>

MedianWindowFilter::MedianWindowFilter(uint16_t FltWindowSize, QObject *parent)
   : QObject(parent)
{
    n_elements = FltWindowSize;

    if(n_elements)
    {
        FilterArray = new double[FltWindowSize];
    }

    for(uint32_t i = 0; i < n_elements; ++i)
        sortList.append(0);

    Reset();
}

MedianWindowFilter::~MedianWindowFilter()
{
    if(FilterArray != nullptr)
    {
        delete FilterArray;
    }

    if(!sortList.isEmpty())
        sortList.clear();
}

bool MedianWindowFilter::Reset()
{
    if(FilterArray == nullptr)
        return false;

    if(!n_elements)
        return false;

    for(uint32_t i = 0; i < n_elements; ++i)
    {
        FilterArray[i] = 0.f;
        sortList[i] = 0.f;
    }

    median = 0.f;
    in = 0;
    m_isFilterFilled = false;

    return true;
}

bool MedianWindowFilter::IsFilled()
{
    return m_isFilterFilled;
}

void MedianWindowFilter::AddElement(double val)
{
    if(FilterArray == nullptr)
        return;

    FilterArray[in] = val;

    if(++in == n_elements)
    {
        in = 0;
        m_isFilterFilled = true;
    }
}

double MedianWindowFilter::Filter()
{
    // Sort from smallest to biggest
    for(uint32_t i = 0; i < n_elements; ++i)
        sortList[i] = FilterArray[i];

    std::sort(sortList.begin(), sortList.end());

    if(!(n_elements % 2))
    {
        // For even window size (e.g. = 10), calculate average of two elements in the middle (e.g. [4] and [5])
        median = (sortList[n_elements/2-1] + sortList[n_elements/2]) / 2;
    }
    else
    {
        // For odd window size, take center element
        median = sortList[n_elements/2];
    }

    return median;
}
