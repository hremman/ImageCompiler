#include "ColorFunctions.h"
#include <QtGlobal>

std::uniform_int_distribution<unsigned char> ColorFunctions::__M_random;
std::random_device ColorFunctions::__M_device;
std::mt19937 ColorFunctions::__M_generator(__M_device);

QImage ColorFunctions::CompositeAlpha(const QImage & img_up, const QImage & img_down)
{

    QImage img_res(qMax(img_up.width(), img_down.width()), qMax(img_up.height(), img_down.height()), img_down.format());
    img_res.fill(QColor(0,0,0,0));

    QPoint down_shift(
                (img_res.width() - img_down.width())/2,
                (img_res.height() - img_down.height())/2
                );

    for (auto col = 0; col < img_down.width(); col++)
    {
        for (auto row = 0; row < img_down.height(); row++)

        {
            img_res.setPixelColor(col + down_shift.rx(), row + down_shift.ry(), img_down.pixelColor(col, row) );
        }
    }

    QPoint up_shift(
                (img_res.width() - img_up.width())/2,
                (img_res.height() - img_up.height())/2
                );

    for (auto col = 0; col < img_up.width(); col++)
    {
        for (auto row = 0; row < img_up.height(); row++)
        {
            QColor p_up = img_up.pixelColor(col, row);
            QColor p_down = img_res.pixelColor(col + up_shift.rx(), row + up_shift.ry());

            if( p_up.alpha() == 0 )
            {
                continue;
            }
            if ( p_down.alpha() == 255 || p_down.alpha() == 0 )
            {
                img_res.setPixelColor(col + up_shift.rx(), row + up_shift.ry(), p_up);
                continue;
            }

            QColor p_res(0,0,0,0);
            p_res.setAlphaF( p_up.alphaF() + p_down.alphaF()*(1-p_up.alphaF()) );
            p_res.setRed((p_up.red()*p_up.alphaF() + p_down.red()*p_down.alphaF()*(1-p_up.alphaF()))/p_res.alphaF() );
            p_res.setGreen((p_up.green()*p_up.alphaF() + p_down.green()*p_down.alphaF()*(1-p_up.alphaF()))/p_res.alphaF());
            p_res.setBlue((p_up.blue()*p_up.alphaF() + p_down.blue()*p_down.alphaF()*(1-p_up.alphaF()))/p_res.alphaF());

            img_res.setPixelColor(col + up_shift.rx(), row + up_shift.ry(), p_res);

        }
    }
    return img_res;
}

QImage ColorFunctions::ChangeColor(const QImage & img, int hue)
{
    QImage img_res(img.width(), img.height(), img.format());
    for (auto col = 0; col < img.width(); col++)
    {
        for (auto row = 0; row < img.height(); row++)
        {
            QColor p = img.pixelColor(col, row);
            p.setHsv(hue % 361, p.saturation(), p.value(), p.alpha());
            img_res.setPixelColor(col, row, p);
        }
    }
    return img_res;
}

QImage ColorFunctions::Fill(const QImage & img)
{
    QImage img_res(img.width(), img.height(), img.format());
    for (auto col = 0; col < img.width(); col++)
    {
        for (auto row = 0; row < img.height(); row++)
        {
            QColor p_i = img.pixelColor(col, row);
            QColor p_r(255,255,255,0);
            if ( p_i.alpha() != 0)
            {
                p_r.setRed(__M_random(__M_generator));
                p_r.setGreen(__M_random(__M_generator));
                p_r.setBlue(__M_random(__M_generator));
                p_r.setAlpha(p_i.alpha());
            }
            img_res.setPixelColor(col, row, p_r);
        }
    }
    return img_res;
}
