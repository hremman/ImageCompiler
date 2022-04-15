#include "ColorFunctions.h"


QColor CColorFunctions::CompositeAlphaPixel(const QColor & top, const QColor &down)
{
    if( top.alpha() == 0 )
    {
        return down;
    }
    if ( top.alpha() == 255 )
        return top;

    QColor res(0,0,0,0);
    res.setAlphaF( top.alphaF() + down.alphaF()*(1-top.alphaF()) );
    res.setRed((top.red()*top.alphaF() + down.red()*down.alphaF()*(1-top.alphaF()))/res.alphaF() );
    res.setGreen((top.green()*top.alphaF() + down.green()*down.alphaF()*(1-top.alphaF()))/res.alphaF());
    res.setBlue((top.blue()*top.alphaF() + down.blue()*down.alphaF()*(1-top.alphaF()))/res.alphaF());
    return res;
}

QColor CColorFunctions::SetHue(const QColor & pixel, int hue)
{
    QColor p;
    p.setHsv(hue % 361, pixel.saturation(), pixel.value(), pixel.alpha());
    return p;
}

QColor CColorFunctions::SetSaturation(const QColor & pixel, const RangeMapper & mapper)
{
    QColor p;
    p.setHsv(pixel.hsvHue(), mapper(pixel.hsvSaturation()), pixel.value(), pixel.alpha());
    return p;
}

QColor CColorFunctions::SetValue(const QColor & pixel, const RangeMapper & mapper)
{
    QColor p;
    p.setHsv(pixel.hsvHue(), pixel.hsvSaturation(), mapper(pixel.value()), pixel.alpha());
    return p;
}
