#include "DataLayer.hpp"

CDataLayer::CDataLayer()
    : m_name("Новый Слой")
    , m_file_mask("*")
    , m_path("")
    , m_one_file(true)
    , m_use_probability(1.0)
    , m_noise_probability(0.0)
    , m_colors()
{

}

