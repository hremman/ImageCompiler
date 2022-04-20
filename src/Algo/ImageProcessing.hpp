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
    bool CompositeAlpha(QImage *, const QImage &, size_t, size_t );
    template <bool SATURATION, bool VALUE>
    bool ChangeColor(QImage *, int, size_t, size_t,  const RangeMapper* saturation = nullptr, const RangeMapper* value = nullptr );

};

#endif // CIMAGEPROCSSING_HPP
