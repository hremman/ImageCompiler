#ifndef OLD_IMAGETRANSFORMATIONINTERFACE_H
#define OLD_IMAGETRANSFORMATIONINTERFACE_H

#include <QImage>
#include <unordered_map>


class CImageTransformInterface
{
public:
    CImageTransformInterface()
    {}

    virtual QImage transform(const QImage &) = 0;
    virtual void reset() = 0;
    virtual bool haveWork() const = 0;

};


#endif // OLD_IMAGETRANSFORMATIONINTERFACE_H
