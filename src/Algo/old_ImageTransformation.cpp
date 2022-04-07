#include "old_ImageTransformation.hpp"

//CNullTransformation

QImage CNullTransform::transform(const QImage & img)
    {return img;}

void CNullTransform::reset()
    {m_done = true;}

bool CNullTransform::haveWork() const
    {return m_done;}


//CPaletteTransformation

CPaletteTransform::CPaletteTransform()
{}

QImage CPaletteTransform::transform(const QImage & img)
    {return img;}

void CPaletteTransform::reset()
    {m_current = m_colors->cbegin();}

bool CPaletteTransform::haveWork() const
    {return m_current != m_colors->cend();}

void CPaletteTransform::setColors(const QList<Data::CColor> * colors)
    {m_colors = colors;}



//CGenerateTransformation

QImage CGenerateTransform::transform(const QImage & img)
    {return img;}

bool CGenerateTransform::haveWork() const
{
    if (m_remember)
        return m_current != m_used_colors.end();
    else
        return m_used_colors.size() < static_cast<size_t>(m_number);
}

void CGenerateTransform::reset()
{
    if (m_remember)
        m_current = m_used_colors.begin();
    else
        m_used_colors.clear();
}
