
#include "Data/Layer.hpp"

Data::CLayer::CLayer(uid_t lid)
    : m_name("Новый Слой")
    , m_files()
    , m_use_probability(1.0)
    , m_noise_probability(0.0)
    , m_type(WorkType::NO_TYPE)
    , m_colors()
    , m_lid(lid)
{

}
Data::CLayer::~CLayer()
{}

bool Data::CLayer::operator == (const CLayer &r)
{
    return m_lid == r.m_lid;
}
