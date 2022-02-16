#ifndef ALGCOLORFUNCTIONS_H
#define ALGCOLORFUNCTIONS_H
#include <QImage>
#include <random>

class ColorFunctions
{
public:

    static QImage CompositeAlpha(const QImage & up, const QImage & down);
    static QImage ChangeColor(const QImage & img, int hue); //TODO Добавить учёт сатурации и значения

};

#endif // ALGCOLORFUNCTIONS_H
