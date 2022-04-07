#ifndef CCOLORFUNCTIONS_H
#define CCOLORFUNCTIONS_H

#include <QColor>

class CColorFunctions
{
public:
    struct AverageRange
    {
        int m_top = 0;
        int m_average = 0;
        int m_down = 0;
    };

    class RangeMapper
    {
    public:
        RangeMapper();
        RangeMapper(const AverageRange &, int);

        __attribute__((always_inline)) int map(int val) const
            { return (double)(val - m_down_old) * m_k + m_down;}
        __attribute__((always_inline)) int operator()(int val) const
            { return map(val);}

    protected:
        double m_k;
        int m_down;
        int m_down_old;


    };

public:
    static QColor CompositeAlphaPixel(const QColor & top, const QColor &down);
    static QColor SetHue(const QColor & pixel, int hue);
    static QColor SetSaturation(const QColor & pixel, const RangeMapper & mapper);
    static QColor SetValue(const QColor & pixel, const RangeMapper & mapper);
};

#endif // CCOLORFUNCTIONS_H
