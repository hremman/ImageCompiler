#ifndef COLOR_H
#define COLOR_H

#include <QColor>
#include "Algo/json.hpp"

namespace Data
{
    struct CColor
    {
    public:
        QColor m_color;
        bool m_saturation;
        bool m_value;


        CColor()
            : m_color(Qt::gray)
            , m_saturation(false)
            , m_value(false)
        {}

        CColor(const CColor & ref)
            : m_color(ref.m_color)
            , m_saturation(ref.m_saturation)
            , m_value(ref.m_value)
        {}

        CColor & operator=(const CColor & ref)
        {
            if ( this != &ref)
            {
                m_color = ref.m_color;
                m_saturation = ref.m_saturation;
                m_value = ref.m_value;
            }
            return *this;

        }

        bool operator == (const CColor & ref)
        {
            return m_color == ref.m_color && m_saturation == ref.m_saturation && m_value == ref.m_value;
        }

        const nlohmann::json to_json()  const
        {
            nlohmann::json json;
            json["color"] = { {"red", m_color.red()}, {"green", m_color.green()}, {"blue", m_color.blue()}};
            json["sat"] = m_saturation;
            json["sav"] = m_value;
            return json;
        }

        void from_jsom(const nlohmann::json & json)
        {
            m_color.setRgb(json.at("color").at("red"), json.at("color").at("green"), json.at("color").at("blue"));
            m_saturation = json.at("sat");
            m_value = json.at("val");
        }

    };





    struct CColorHash{
    public:
        size_t operator()(const CColor & color) const
        {
            return static_cast<uint>(color.m_saturation) | static_cast<uint>(color.m_value) << 1 | color.m_color.red() << 2 | color.m_color.green() << 10 | color.m_color.blue() << 18;
        }

    };
}
#endif // COLOR_H
