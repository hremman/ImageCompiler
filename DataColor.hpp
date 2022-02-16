#ifndef DATACOLOR_H
#define DATACOLOR_H

#include <QColor>
#include "FileFormat.hpp"


class CDataColor
{
public:
    QColor m_color;
    bool m_saturation;
    bool m_value;
};

#endif // DATACOLOR_H
