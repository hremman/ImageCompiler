#include "Exceptions.hpp"
#include "ColorSettings.h"

Data::CColorSettings::CColorSettings()
    : m_generations_number(10)
    , m_colors()
    , m_saturatiom(false)
    , m_value(false)
    , m_for_each(false)
    , m_mode(Mode::GENERATION)
{}


Data::CColorSettings::CColorSettings(const CColorSettings  * ptr)
    : m_generations_number(ptr->m_generations_number)
    , m_colors(ptr->m_colors)
    , m_saturatiom(ptr->m_saturatiom)
    , m_value(ptr->m_value)
    , m_for_each(ptr->m_for_each)
    , m_mode(ptr->m_mode)
{}

Data::CColorSettings::CColorSettings(const CColorSettings & ref)
    : m_generations_number(ref.m_generations_number)
    , m_colors(ref.m_colors)
    , m_saturatiom(ref.m_saturatiom)
    , m_value(ref.m_value)
    , m_for_each(ref.m_for_each)
    , m_mode(ref.m_mode)
{}

const nlohmann::json Data::CColorSettings::to_json()  const
{
    nlohmann::json json;
    json["mode"] = m_mode;
    if (m_mode == Data::CColorSettings::Mode::GENERATION)
    {
        json["gen_num"] = m_generations_number;
        json["sat"] = m_saturatiom;
        json["val"] = m_value;
        json["for_each"] = m_for_each;
    }
    else
    {
        for (auto it = m_colors.begin(); it != m_colors.end(); it++)
            json["colors"].push_back(it->to_json());
    }
    return json;
}

void Data::CColorSettings::from_jsom(const nlohmann::json & json)
{
    m_mode = json.at("mode");
    if ( json.at("mode") == Data::CColorSettings::Mode::GENERATION)
    {
        json.at("gen_num").get_to(m_generations_number);
        json.at("sat").get_to(m_saturatiom);
        json.at("val").get_to(m_value);
        json.at("for_each").get_to(m_for_each);
    }
    else if(json.at("mode") == Data::CColorSettings::Mode::ENUMERATION)
    {
        m_colors.clear();
        for (auto it = json.at("colors").begin(); it != json.end(); ++it)
            m_colors.emplace_back().from_jsom(*it);
    }
    else
        throw wrong_mode("Ошибка разбора файла: неверное значение \"mode\" объекта \"colors_settings\". Допустимы: [0, 1]");

}

size_t  Data::CColorSettings::count() const
{
     return m_mode == Mode::ENUMERATION ? m_colors.size() : m_generations_number;
}
