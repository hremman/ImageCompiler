
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
const nlohmann::json Data::CLayer::to_json() const
{
    nlohmann::json json = {{"name",m_name.toStdString()},
                           {"use", m_use_probability},
                           {"noise", m_noise_probability},
                           {"type", m_type},
                           {"lid", m_lid}
                          };
    if (m_type == WorkType::ENUMERATION)
        json["colors"] = m_colors.to_json();
    for (auto i: m_files)
        json["files"].push_back(i.toStdString());

    return json;
}

void Data::CLayer::from_jsom(const nlohmann::json & json)
{
    m_name.fromStdString(json.at("name").get<std::string>());
    json.at("use").get_to(m_use_probability);
    json.at("noise").get_to(m_noise_probability);
    json.at("lid").get_to(m_lid);
    json.at("type").get_to(m_type);
    if (m_type < WorkType::NO_TYPE || m_type > WorkType::ENUMERATION)
        throw wrong_val("Wrong layer type in json");
    if (m_type == WorkType::ENUMERATION)
        m_colors.from_jsom(json.at("colors"));
}
size_t  Data::CLayer::count() const
{
    if ( ! m_files.count() )
        return 0;
    size_t temp = m_files.count() * (m_type == WorkType::ENUMERATION ? m_colors.count() : 1);
    return temp + (m_noise_probability == 0 ? 0 : 1);
}
