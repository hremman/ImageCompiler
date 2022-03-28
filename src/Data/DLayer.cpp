#include "Data/DLayer.hpp"

Data::CLayer::CLayer()
    : m_name("Новый Слой")
    , m_files()
    , m_use_probability(1.0)
    , m_noise_probability(0.0)
    , m_generations_number(0)
    , m_colors()
    , m_type(WorkType::NO_TYPE)
{

}

