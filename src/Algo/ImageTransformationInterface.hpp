#ifndef IMAGETRANSFORMATIONINTERFACE_HPP
#define IMAGETRANSFORMATIONINTERFACE_HPP

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


#endif // IMAGETRANSFORMATIONINTERFACE_HPP
