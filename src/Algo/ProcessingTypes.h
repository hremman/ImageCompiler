#ifndef PROCESSINGTYPES_H
#define PROCESSINGTYPES_H

#include <stdexcept>

struct AverageRange
{
    int m_top = 0;
    int m_average = 0;
    int m_down = 100;
};

class RangeMapper
{
public:
    RangeMapper()
        : m_k(0)
        , m_down(0)
        , m_down_old(0)
    {}
    RangeMapper(const AverageRange & range, int new_val)
        : RangeMapper()
    {
        AverageRange new_range;
        new_range.m_average= new_val - range.m_average;
        new_range.m_top = range.m_top + new_range.m_average;
        new_range.m_down = range.m_top + new_range.m_average;
        if ( new_range.m_top > 100 ) new_range.m_top = 100;
        if ( new_range.m_down >= 100 ) new_range.m_down = 95;
        if ( new_range.m_down < 0 ) new_range.m_down = 0;
        if ( new_range.m_top <= 0 ) new_range.m_top = 95;

        m_k = (double)(new_range.m_top - new_range.m_down) / (double)(range.m_top - range.m_down);

    }

    __attribute__((always_inline)) int map(int val) const
        { return (double)(val - m_down_old) * m_k + m_down;}
    __attribute__((always_inline)) int operator()(int val) const
        { return map(val);}

protected:
    double m_k;
    int m_down;
    int m_down_old;
};

class nullpr_arg : std::exception
{

};



#endif // PROCESSINGTYPES_H
