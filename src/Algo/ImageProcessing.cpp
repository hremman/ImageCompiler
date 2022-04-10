#include <QThreadPool>
#include "Exceptions.hpp"
#include "ImageProcessing.hpp"
#include "ColorFunctions.h"



AverageRange CImageProcessing::getAverageSaturation(const QImage & image, size_t num, size_t count)
{
    AverageRange ret;
    unsigned long long average = 0;
    size_t rows = 0;
    for (int row = num; row < image.height(); row += count)
    {
        rows++;
        for (int col = 0; col < image.width(); col ++)
        {
            const QColor & pixel = image.pixelColor(col, row);
            if ( pixel.hsvSaturation() < ret.m_down)
                ret.m_down = pixel.hsvSaturation();
            if ( pixel.hsvSaturation() > ret.m_top)
                ret.m_top = pixel.hsvSaturation();
            average +=  pixel.hsvSaturation();
        }
    }
    ret.m_average = average/(rows == 0 ? 1 : rows * image.width());
    return ret;
}

AverageRange getAverageValue(const QImage & image, size_t num, size_t count)
{
    AverageRange ret;
    unsigned long long average = 0;
    size_t rows = 0;
    for (int row = num; row < image.height(); row += count)
    {
        rows++;
        for (int col = 0; col < image.width(); col ++)
        {
            const QColor & pixel = image.pixelColor(col, row);
            if ( pixel.value() < ret.m_down)
                ret.m_down = pixel.value();
            if ( pixel.value() > ret.m_top)
                ret.m_top = pixel.value();
            average +=  pixel.value();
        }
    }
    ret.m_average = average/(rows == 0 ? 1 : rows * image.width());
    return ret;
}

bool CompositeAlpha(QImage *down, const QImage &top, size_t num, size_t count )
{

    for (int row = num; row < down->height(); row += count)
    {
        for (int col = 0; col < down->width(); col ++)
        {
            ;
        }
    }
    return true;
}

template <bool SATURATION, bool VALUE>
bool ChangeColor(QImage * image, int hue, size_t num, size_t count,  const RangeMapper* saturation = nullptr, const RangeMapper* value = nullptr )
{
    if (SATURATION && saturation == nullptr)
        throw nullpointer("Нулевой указатель на необходимый объект RangeMapper для насыщенности");

    if (VALUE && saturation == nullptr)
        throw nullpointer("Нулевой указатель на необходимый объект RangeMapper для яркости");

    for (int row = num; row < image->height(); row += count)
    {
        for (int col = 0; col < image->width(); col ++)
        {
            if (SATURATION)
            {
                image->setPixelColor(col, row, CColorFunctions::SetSaturation(image->pixelColor(col, row), *saturation));
            }
            if (VALUE)
            {
                image->setPixelColor(col, row, CColorFunctions::SetValue(image->pixelColor(col, row), *value));
            }
            image->setPixelColor(col, row, CColorFunctions::SetHue(image->pixelColor(col, row), hue));
        }
    }
}






