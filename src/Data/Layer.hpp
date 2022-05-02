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
        const nlohmann::json to_json(const QString &)  const;
        void from_jsom(const nlohmann::json & json);
        size_t count() const;

    protected:  //fields
        uid_t m_lid;
    };
}
#endif // CDATALAYER_HPP
