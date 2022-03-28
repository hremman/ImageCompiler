#include "ImageTransformation.hpp"

//CNullTransformation

QImage CNullTransformation::transform(const QImage & img)
    {return img;}

void CNullTransformation::reset()
    {m_done = true;}

bool CNullTransformation::haveWork() const
    {return m_done;}


//CPaletteTransformation

CPaletteTransformation::CPaletteTransformation()
{}

QImage CPaletteTransformation::transform(const QImage & img)
    {return img;}

void CPaletteTransformation::reset()
    {m_current = m_colors->cbegin();}

bool CPaletteTransformation::haveWork() const
    {return m_current != m_colors->cend();}

void CPaletteTransformation::setColors(const QList<Data::CColor> * colors)
    {m_colors = colors;}



//CGenerateTransformation

QImage CGenerateTransformation::transform(const QImage & img)
    {return img;}

bool CGenerateTransformation::haveWork() const
{
    if (m_remember)
        return m_current != m_used_colors.end();
    else
        return m_used_colors.size() < m_number;
}

void CGenerateTransformation::reset()
{
    if (m_remember)
        m_current = m_used_colors.begin();
    else
        m_used_colors.clear();
}
