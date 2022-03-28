#ifndef CDATALAYER_HPP
#define CDATALAYER_HPP

#include <QList>
#include "Color.hpp"
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

    public: //fields
        QString m_name;
        QStringList m_files;
        double m_use_probability;
        double m_noise_probability;
        int m_generations_number;
        QList<CColor> m_colors;
        WorkType m_type;

    public:
        CLayer();



    };
}
#endif // CDATALAYER_HPP
