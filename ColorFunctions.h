#ifndef COLORFUNCTIONS_H
#define COLORFUNCTIONS_H
#include <QImage>
#include <random>

class ColorFunctions
{
private:
    static std::uniform_int_distribution<unsigned char> __M_random;
    static std::mt19937 __M_generator;
    static std::random_device __M_device;
public:

    static QImage CompositeAlpha(const QImage & up, const QImage & down);
    static QImage ChangeColor(const QImage & img, int hue);
    static QImage Fill(const QImage & img);

};

#endif // COLORFUNCTIONS_H
