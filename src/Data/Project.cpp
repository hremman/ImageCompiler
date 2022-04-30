#include "Exceptions.hpp"
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
        throw too_many_id("Превышен лимит идентификаторов для объекта Data::CLayer");

    return *(m_used_lid.emplace(prev + 1).first);
}

Data::CProject::CProject()
    : m_name("Новый проект")
    , m_out_path("./")
    , m_file("")
    , m_used_lid()
    , m_layers()
{

}

Data::CProject::~CProject()
{
}



Data::CProject::CProject(const CProject& ref)
    : m_name(ref.m_name)
    , m_out_path(ref.m_out_path)
    , m_used_lid(ref.m_used_lid)
    , m_layers(ref.m_layers)
{}

Data::CProject& Data::CProject::operator =(const CProject & ref)
{
    if (&ref != this)
    {
        m_name = ref.m_name;
        m_out_path = ref.m_out_path;
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

const std::list<Data::CLayer*> & Data::CProject::layers() const
{
    return m_layers;
}


const nlohmann::json Data::CProject::to_json()  const
{
    nlohmann::json json = {
        {"name", m_name.toStdString()},
        {"out", m_out_path.toStdString()},
        {"version", (static_cast<unsigned>(__M_major_ver) << 16) | __M_minor_ver },
    };

    for (auto it = m_layers.begin(); it != m_layers.end(); it++)
        json["layers"].push_back( (*it)->to_json() );

    return json;
}

void Data::CProject::from_jsom(const nlohmann::json & json)
{
    unsigned version;
    json.at("version").get_to(version);
    if ( (version ^ ((static_cast<unsigned>(__M_major_ver) << 16) | __M_minor_ver)) != 0 )
        throw unsupported_version("Неподдерживаемая версия (" + QString::number( version >> 16) + "." + QString::number( version & 0xFFFF) + ") проекта");
    m_name.fromStdString(json.at("name"));
    m_out_path.fromStdString(json.at("out"));
    m_layers.clear();
    m_used_lid.clear();
    for (auto it = json.at("layers").begin(); it != json.end(); ++it)
    {
        m_layers.emplace_back(new CLayer(it->at("lid")));
        m_layers.back()->from_jsom(*it);
        m_used_lid.insert(m_layers.back()->get_lid());
    }

}

size_t Data::CProject::count() const
{
    std::vector<size_t> ret_v;
    for (auto it = m_layers.begin(); it != m_layers.end(); it++ )
    {
        size_t temp = (*it)->count();
        if (temp)
            ret_v.push_back(temp);
    }
    if (ret_v.size() == 0 )
    {
        return 0;
    }

    size_t ret = 1;
    for(auto it: ret_v)
        ret *= it;
    return ret;
}
