#include "ColorFunctions.h"

#include <QDebug>

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
    if (hue < -1 || pixel.value() > 255 || pixel.value() < 0 || pixel.alpha() > 255 || pixel.alpha() < 0 || pixel.hsvSaturation() > 255 || pixel.hsvSaturation() < 0)
        qDebug() << QString("[SetSaturation]Hue: %1; Sat: %2; Val: %3; Alpha: %4")
                    .arg(hue)
                    .arg(pixel.hsvSaturation())
                    .arg(pixel.value())
                    .arg(pixel.alpha());
    p.setHsv(hue % 361, pixel.hsvSaturation(), pixel.value(), pixel.alpha());
    return p;
}

QColor CColorFunctions::SetSaturation(const QColor & pixel, const RangeMapper & mapper)
{
    QColor p;
    if (pixel.hsvHue() < -1 || pixel.value() > 255 || pixel.value() < 0 || pixel.alpha() > 255 || pixel.alpha() < 0 || mapper(pixel.hsvSaturation()) > 255 || mapper(pixel.hsvSaturation()) < 0)
        qDebug() << QString("[SetSaturation]Hue: %1; Sat: %2; Val: %3; Alpha: %4")
                    .arg(pixel.hsvHue())
                    .arg(mapper(pixel.hsvSaturation()))
                    .arg(pixel.value())
                    .arg(pixel.alpha());
    p.setHsv(pixel.hsvHue(), mapper(pixel.hsvSaturation()), pixel.value(), pixel.alpha());
    return p;
}

QColor CColorFunctions::SetValue(const QColor & pixel, const RangeMapper & mapper)
{
    QColor p;
    if (pixel.hsvHue() < -1 || mapper(pixel.value()) > 255 || mapper(pixel.value()) < 0 || pixel.alpha() > 255 || pixel.alpha() < 0 || pixel.hsvSaturation() > 255 || pixel.hsvSaturation() < 0)
        qDebug() << QString("[SetSaturation]Hue: %1; Sat: %2; Val: %3; Alpha: %4")
                    .arg(pixel.hsvHue())
                    .arg(pixel.hsvSaturation())
                    .arg(mapper(pixel.value()))
                    .arg(pixel.alpha());
    p.setHsv(pixel.hsvHue(), pixel.hsvSaturation(), mapper(pixel.value()), pixel.alpha());
    return p;
}

