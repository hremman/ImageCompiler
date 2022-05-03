#ifndef CIMAGEPROCSSING_HPP
#define CIMAGEPROCSSING_HPP
#include <QImage>
#include "ProcessingTypes.h"

namespace CImageProcessing
{
//public:
    AverageRange getAverageSaturation(const QImage &, size_t, size_t);
    AverageRange getAverageValue(const QImage &, size_t, size_t);
    bool getAverage(const QImage &, size_t, size_t, AverageRange &, AverageRange &);
    void CompositeAlpha(QImage *, const QImage &, size_t, size_t );
    void ChangeColor(QImage *, int, size_t, size_t,  const RangeMapper* saturation = nullptr, const RangeMapper* value = nullptr );
    void WhiteNoise(QImage *, size_t, size_t,  const ImageCache &);
    QImage PaddToSize(const QImage &, const QSize &);
    ImageCache CollapseCache(ImageCache *, size_t);
};

#endif // CIMAGEPROCSSING_HPP
