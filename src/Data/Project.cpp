#include "Data/Project.hpp"


Data::CProject::uid_t Data::CProject::new_lid()
{
    uid_t prev = -1;
    for ( auto it = m_used_lid.begin(); it != m_used_lid.end(); it++)
    {
        if (prev + 1 < *it )
        {
            return *(m_used_lid.emplace(prev + 1).first);
        }
        prev = *it;
    }
    if ( prev == 9999)
        throw QString("Превышен лимит идентификаторов");

    return *(m_used_lid.emplace(prev + 1).first);
}

Data::CProject::CProject()
    : m_name("Новый проект")
    , m_out_path(".\\")
    , m_root_path(".\\")
    , m_used_lid()
    , m_layers()
{

}

Data::CProject::CProject(const CProject& ref)
    : m_name(ref.m_name)
    , m_out_path(ref.m_out_path)
    , m_root_path(ref.m_root_path)
    , m_used_lid(ref.m_used_lid)
    , m_layers(ref.m_layers)
{}

Data::CProject& Data::CProject::operator =(const CProject & ref)
{
    if (&ref != this)
    {
        m_name = ref.m_name;
        m_out_path = ref.m_out_path;
        m_root_path = ref.m_root_path;
        m_used_lid = ref.m_used_lid;
        m_layers = ref.m_layers;
    }
    return *this;
}

Data::CLayer * Data::CProject::newLayer()
{
    m_layers.emplace_back(new CLayer(new_lid()));
    return m_layers.back();

}

void Data::CProject::removeLayer(CLayer *layer)
{
    size_t old = m_layers.size();
    m_layers.remove(layer);
    if ( old - 1 == m_layers.size())
    {
        m_used_lid.erase(layer->get_lid());
        delete layer;
    }

}

void  Data::CProject::moveLayerUp(size_t row)
{
    if (row == 0)
        return;
    auto i = m_layers.begin();
    for (size_t j = 0; j <= row; j++)
    {
        i++;
    }
    auto p = *i;
    m_layers.insert(--i, p);
}

void  Data::CProject::moveLayerDown(size_t row)
{
    if (row == m_layers.size())
        return;
    auto i = m_layers.begin();
    for (size_t j = 0; j <= row; j++)
    {
        i++;
    }
    auto p = *i;
    m_layers.insert(++i, p);
}


