#ifndef CIMAGETRANSFORMATION_HPP
#define CIMAGETRANSFORMATION_HPP


#include <QList>
#include <unordered_set>
#include "ImageTransformationInterface.hpp"
#include "Data/Color.hpp"

class CNullTransformation : public CImageTransformationInterface
{

public:
    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;

protected:
    bool m_done;

};


class CGenerateTransformation : public CImageTransformationInterface
{

public: //fields
    int m_number;
    bool m_remember;

public: //methods
    CGenerateTransformation();

    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;


protected: //fields
    std::unordered_set<Data::CColor, Data::CColorHash> m_used_colors;
    std::unordered_set<Data::CColor, Data::CColorHash>::const_iterator m_current;
};

class CPaletteTransformation : public CImageTransformationInterface
{

public: //methods
    CPaletteTransformation();

    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;
    void setColors(const QList<Data::CColor> *);


protected: //fields
    const QList<Data::CColor> * m_colors;
    QList<Data::CColor>::ConstIterator m_current;
};

using CImageTransformation = CImageTransformationInterface;


#endif // CIMAGETRANSFORMATION_HPP
