#ifndef CIMAGETRANSFORMATION_HPP
#define CIMAGETRANSFORMATION_HPP


#include <QList>
#include <unordered_set>
#include "old_ImageTransformationInterface.hpp"
#include "Data/Color.hpp"

class CNullTransform : public CImageTransformInterface
{

public:
    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;

protected:
    bool m_done;

};


class CGenerateTransform : public CImageTransformInterface
{

public: //fields
    int m_number;
    bool m_remember;

public: //methods
    CGenerateTransform();

    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;


protected: //fields
    std::unordered_set<Data::CColor, Data::CColorHash> m_used_colors;
    std::unordered_set<Data::CColor, Data::CColorHash>::const_iterator m_current;
};

class CPaletteTransform : public CImageTransformInterface
{

public: //methods
    CPaletteTransform();

    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;
    void setColors(const QList<Data::CColor> *);


protected: //fields
    const QList<Data::CColor> * m_colors;
    QList<Data::CColor>::ConstIterator m_current;
};

using CImageTransform = CImageTransformInterface;


#endif // CIMAGETRANSFORMATION_HPP
