#ifndef CIMAGETRANSFORMATION_HPP
#define CIMAGETRANSFORMATION_HPP


#include <QList>
#include <QSet>
#include "ImageTransformationInterface.hpp"
#include "DataColor.hpp"

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
    QSet<QColor> m_used_colors;
    QSet<QColor>::Iterator m_current;
};

class CPaletteTransformation : public CImageTransformationInterface
{

public: //methods
    CPaletteTransformation();

    QImage transform(const QImage &) override;
    void reset() override;
    bool haveWork() const override;
    void setColors(const QList<CDataColor> *);


protected: //fields
    const QList<CDataColor> * m_colors;
    QList<CDataColor>::ConstIterator m_current;
};

using CImageTransformation = CImageTransformationInterface;


#endif // CIMAGETRANSFORMATION_HPP
