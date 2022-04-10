#ifndef CIMAGEPROCSSING_HPP
#define CIMAGEPROCSSING_HPP
#include <QImage>
#include "ProcessingTypes.h"

class CImageProcessing
{
public:
    static AverageRange getAverageSaturation(const QImage &, size_t, size_t);
    static AverageRange getAverageValue(const QImage &, size_t, size_t);
    template <bool SATURATION, bool VALUE>
    static void getAverageValue(const QImage &, size_t, size_t, AverageRange &, AverageRange &);
    static void CompositeAlpha(QImage *, const QImage &, size_t, size_t );
    template <bool SATURATION, bool VALUE>
    static void ChangeColor(QImage *, int, size_t, size_t,  const RangeMapper* saturation = nullptr, const RangeMapper* value = nullptr );

};

#endif // CIMAGEPROCSSING_HPP
