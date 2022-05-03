#include <QPainter>
#include <random>
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

void CImageProcessing::WhiteNoise(QImage * image, size_t num, size_t count,  const ImageCache & cache)
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::WhiteNoise]: Передано число потоков обработки меньше одного");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> color_gen;

    for (int row = num; row < image->height(); row += count)
    {
        for (int col = 0; col < image->width(); col ++)
        {
            QColor color(color_gen(gen),color_gen(gen),color_gen(gen));
            RangeMapper sat(cache.m_saturation, color.saturation());
            RangeMapper val(cache.m_value, color.value());
            QColor pixel = image->pixelColor(col, row);
            CColorFunctions::SetSaturation(pixel, sat);
            CColorFunctions::SetValue(pixel, val);
            image->setPixelColor(col, row, CColorFunctions::SetHue(pixel, color.hsvHue()));
        }
    }


}

void CImageProcessing::ChangeColor(QImage * image, int hue, size_t num, size_t count,  const RangeMapper* saturation, const RangeMapper* value )
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::ChangeColor]: Передано число потоков обработки меньше одного");

    if (hue < -1)
        hue = -1;
    for (int row = num; row < image->height(); row += count)
    {
        for (int col = 0; col < image->width(); col ++)
        {
            QColor pixel = image->pixelColor(col, row);
            if (saturation)
                CColorFunctions::SetSaturation(pixel, *saturation);
            if (value)
                CColorFunctions::SetValue(pixel, *value);
            image->setPixelColor(col, row, CColorFunctions::SetHue(pixel, hue));
        }
    }
}

bool CImageProcessing::getAverage(const QImage & image, size_t num, size_t count, AverageRange & saturation, AverageRange & value)
{
    if ( count < 1)
        throw wrong_arg("[CImageProcessing::getAverageValue]: Передано число потоков обработки меньше одного");

    unsigned long long average_s = 0;
    unsigned long long average_v = 0;
    size_t rows = 0;
    for (int row = num; row < image.height(); row += count)
    {
        rows++;
        for (int col = 0; col < image.width(); col ++)
        {
            const QColor & pixel = image.pixelColor(col, row);

            if ( pixel.hsvSaturation() < saturation.m_down)
                saturation.m_down = pixel.hsvSaturation();
            if ( pixel.hsvSaturation() > saturation.m_top)
                saturation.m_top = pixel.hsvSaturation();
            average_s +=  pixel.hsvSaturation();

            if ( pixel.value() < value.m_down)
                value.m_down = pixel.value();
            if ( pixel.value() > value.m_top)
                value.m_top = pixel.value();
            average_v +=  pixel.value();

        }
    }
    saturation.m_average = average_s/((rows == 0 ? 1 : rows) * image.width());
    value.m_average = average_v/((rows == 0 ? 1 : rows) * image.width());
    return true;
}

QImage CImageProcessing::PaddToSize(const QImage & img, const QSize & new_size)
{
    if ( img.size().height() > new_size.height() || img.size().width() > new_size.width() )
        throw wrong_arg("[CImageProcessing::PaddToSize]: Новый размер меньше изначального");
    QImage new_img(new_size, img.format());
    new_img.fill(QColor(0,0,0,0));
    QPainter p(&new_img);
    QSize padded = new_size - img.size();
    p.drawImage(padded.width() / 2,padded.height() / 2,img);
    return new_img;
}

ImageCache CImageProcessing::CollapseCache(ImageCache * cache, size_t size)
{
    ImageCache fin;
    for (size_t i = 0 ; i < size; i++)
    {
        if ( fin.m_saturation.m_down > cache[i].m_saturation.m_down )
            fin.m_saturation.m_down = cache[i].m_saturation.m_down;
        if ( fin.m_value.m_down > cache[i].m_value.m_down )
            fin.m_value.m_down = cache[i].m_value.m_down;
        if ( fin.m_saturation.m_top < cache[i].m_saturation.m_top )
            fin.m_saturation.m_top = cache[i].m_saturation.m_top;
        if ( fin.m_value.m_top < cache[i].m_value.m_top )
            fin.m_value.m_top = cache[i].m_value.m_top;
        fin.m_saturation.m_average += cache[i].m_saturation.m_average;
        fin.m_value.m_average += cache[i].m_value.m_average;
    }
    fin.m_saturation.m_average /= size;
    fin.m_value.m_average /= size;

    return fin;
}
