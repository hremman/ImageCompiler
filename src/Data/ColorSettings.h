#ifndef CCOLORSETTINGS_H
#define CCOLORSETTINGS_H
#include <list>

#include "Data/Color.hpp"


namespace Data {

class CColorSettings
{
public:
    enum class Mode{
        GENERATION = 1,
        ENUMERATION = 2
    };

    CColorSettings();
    CColorSettings(const CColorSettings  * ptr);

    int m_generations_number;
    std::list<CColor> m_colors;
    bool m_saturatiom;
    bool m_value;
    Mode m_mode;
};
}

#endif // CCOLORSETTINGS_H
