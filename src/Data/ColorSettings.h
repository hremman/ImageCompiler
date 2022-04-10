#ifndef CCOLORSETTINGS_H
#define CCOLORSETTINGS_H
#include <list>

#include "Data/Color.hpp"
#include "Algo/json.hpp"


namespace Data {

    class CColorSettings
    {
    public:
        enum class Mode{
            GENERATION = 0,
            ENUMERATION = 1
        };

        CColorSettings();
        CColorSettings(const CColorSettings  * ptr);

        int m_generations_number;
        std::list<CColor> m_colors;
        bool m_saturatiom;
        bool m_value;
        Mode m_mode;

        const nlohmann::json to_json()  const;
        void from_jsom(const nlohmann::json & json);
        size_t  count() const;
    };

}

#endif // CCOLORSETTINGS_H
