#ifndef CDATALAYER_HPP
#define CDATALAYER_HPP

#include <QList>
#include <set>
#include <stdexcept>
#include "Data/ColorSettings.h"
#include "File/Formats.hpp"

namespace Data {

    class CLayer
    {

    public: //types
        enum class WorkType{
            NOTHING = 0,
            GENERATION = 1,
            ENUMERATION = 2,
            NO_TYPE = -1
        };
        using uid_t = unsigned short;

    public: //fields
        QString m_name;
        QStringList m_files;
        double m_use_probability;
        double m_noise_probability;
        WorkType m_type;
        CColorSettings m_colors;

    public:
        CLayer(uid_t);
        ~CLayer();
        uid_t get_lid() const
            {return m_lid;}
        bool operator == (const CLayer &);


    protected:  //fields
        uid_t m_lid;
    };
}
#endif // CDATALAYER_HPP
