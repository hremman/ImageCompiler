
#include "Data/Layer.hpp"


std::set<Data::CLayer::uid_t> Data::CLayer::__M_used_lid;



Data::CLayer::uid_t Data::CLayer::new_lid()
{
    uid_t prev = -1;
    for ( auto it = __M_used_lid.begin(); it != __M_used_lid.end(); it++)
    {
        if (prev + 1 < *it )
        {
            return *(__M_used_lid.emplace(prev + 1).first);
        }
        prev = *it;
    }
    if ( prev == 9999)
        throw QString("Превышен лимит идентификаторов");

    return *(__M_used_lid.emplace(prev + 1).first);
}

Data::CLayer::CLayer()
    : m_name("Новый Слой")
    , m_files()
    , m_use_probability(1.0)
    , m_noise_probability(0.0)
    , m_type(WorkType::NO_TYPE)
    , m_colors()
    , m_lid(new_lid())
{

}
Data::CLayer::~CLayer()
{
    __M_used_lid.erase(m_lid);
}
