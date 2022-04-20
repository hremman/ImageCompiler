#ifndef CDATALAYER_HPP
#define CDATALAYER_HPP

#include <QList>
#include <set>
#include <stdexcept>
#include "Data/ColorSettings.h"

namespace Data {

    class CLayer
    {

    public: //types
        enum class WorkType{
            NO_TYPE = -1,
            NOTHING = 0,
            ENUMERATION = 1

        };
        using uid_t = unsigned short;
        class CState
        {
        public:
            CState()
                : m_current_file(-1)
                , m_palete()
                , m_current_color(-1)
            {}

            int m_current_file;
            std::list<Data::CColor> m_palete;
            int m_current_color;
        };

    public: //fields
        QString m_name;
        QStringList m_files;
        double m_use_probability;
        double m_noise_probability;
        bool m_blink;
        WorkType m_type;
        CColorSettings m_colors;

    public:
        CLayer(uid_t);
        ~CLayer();
        uid_t get_lid() const
            {return m_lid;}
        bool operator == (const CLayer &);
        const nlohmann::json to_json()  const;
        void from_jsom(const nlohmann::json & json);
        size_t count() const;

    protected:  //fields
        uid_t m_lid;
    };
}
#endif // CDATALAYER_HPP
