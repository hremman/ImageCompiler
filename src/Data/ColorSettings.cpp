#include "ColorSettings.h"

Data::CColorSettings::CColorSettings()
    : m_generations_number(10)
    , m_colors()
    , m_saturatiom(false)
    , m_value(false)
    , m_mode(Mode::GENERATION)
{}


Data::CColorSettings::CColorSettings(const CColorSettings  * ptr)
    : m_generations_number(ptr->m_generations_number)
    , m_colors(ptr->m_colors)
    , m_saturatiom(ptr->m_saturatiom)
    , m_value(ptr->m_value)
    , m_mode(ptr->m_mode)
{}
