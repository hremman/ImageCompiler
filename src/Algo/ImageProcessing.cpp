#include <QThreadPool>
#include "Exceptions.hpp"
#include "ImageProcessing.hpp"
#include "ColorFunctions.h"



AverageRange CImageProcessing::getAverageSaturation(const QImage & image, size_t num, size_t count)
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::getAverageSaturation]: Передано число потоков обработки меньше одного");
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

AverageRange CImageProcessing::getAverageValue(const QImage & image, size_t num, size_t count)
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::getAverageValue]: Передано число потоков обработки меньше одного");
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
    ret.m_average = average/((rows == 0 ? 1 : rows) * image.width());
    return ret;
}

void CImageProcessing::CompositeAlpha(QImage *down, const QImage &top, size_t num, size_t count )
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::CompositeAlpha]: Передано число потоков обработки меньше одного");
    int h_shift = 0, v_shift = 0;
    if ( top.height() < down->height() )
        h_shift = (down->height() - top.height()) / 2;
    if ( top.width() < down->width() )
        v_shift = (down->width() - top.width()) / 2;

    for (int row = num; row < top.height(); row += count)
        for (int col = 0; col < top.width(); col ++)
            down->setPixelColor(col + v_shift, row + h_shift, CColorFunctions::CompositeAlphaPixel(top.pixelColor(col, row), down->pixelColor(col + v_shift, row + h_shift)));

}

template <bool SATURATION, bool VALUE>
void CImageProcessing::ChangeColor(QImage * image, int hue, size_t num, size_t count,  const RangeMapper* saturation, const RangeMapper* value )
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::ChangeColor]: Передано число потоков обработки меньше одного");
    if (SATURATION && saturation == nullptr)
        throw nullpointer("Получен нулевой указатель на необходимый объект RangeMapper для насыщенности");

    if (VALUE && saturation == nullptr)
        throw nullpointer("Получен нулевой указатель на необходимый объект RangeMapper для яркости");

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

template <bool SATURATION, bool VALUE>
void CImageProcessing::getAverageValue(const QImage & image, size_t num, size_t count, AverageRange & saturation, AverageRange & value)
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::getAverageValue]: Передано число потоков обработки меньше одного");
    if ( !SATURATION && !VALUE)
        return;
    unsigned long long average_s = 0;
    unsigned long long average_v = 0;
    size_t rows = 0;
    for (int row = num; row < image.height(); row += count)
    {
        rows++;
        for (int col = 0; col < image.width(); col ++)
        {
            const QColor & pixel = image.pixelColor(col, row);
            if (SATURATION)
            {
                if ( pixel.hsvSaturation() < saturation.m_down)
                    saturation.m_down = pixel.hsvSaturation();
                if ( pixel.hsvSaturation() > saturation.m_top)
                    saturation.m_top = pixel.hsvSaturation();
                average_s +=  pixel.hsvSaturation();
            }
            if (VALUE)
            {
                if ( pixel.value() < value.m_down)
                    value.m_down = pixel.value();
                if ( pixel.value() > value.m_top)
                    value.m_top = pixel.value();
                average_v +=  pixel.value();
            }
        }
    }
    saturation.m_average = average_s/((rows == 0 ? 1 : rows) * image.width());
    value.m_average = average_v/((rows == 0 ? 1 : rows) * image.width());
}




