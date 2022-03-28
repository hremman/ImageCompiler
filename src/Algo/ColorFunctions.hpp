#ifndef COLORFUNCTIONS_H
#define COLORFUNCTIONS_H
#include <QImage>
#include <random>

class ColorFunctions
{
public:

    static QImage CompositeAlpha(const QImage & up, const QImage & down);
    static QImage ChangeColor(const QImage & img, int hue); //TODO Р”РѕР±Р°РІРёС‚СЊ СѓС‡С‘С‚ СЃР°С‚СѓСЂР°С†РёРё Рё Р·РЅР°С‡РµРЅРёСЏ

};

#endif // COLORFUNCTIONS_H
