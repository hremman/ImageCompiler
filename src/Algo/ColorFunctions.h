#ifndef CCOLORFUNCTIONS_H
#define CCOLORFUNCTIONS_H

#include <QColor>

#include "ProcessingTypes.h"

class CColorFunctions
{
public:
    static QColor CompositeAlphaPixel(const QColor & top, const QColor &down);
    static QColor SetHue(const QColor & pixel, int hue);
    static QColor SetSaturation(const QColor & pixel, const RangeMapper & mapper);
    static QColor SetValue(const QColor & pixel, const RangeMapper & mapper);
};

#endif // CCOLORFUNCTIONS_H
