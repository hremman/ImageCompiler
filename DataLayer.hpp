#ifndef CDATALAYER_HPP
#define CDATALAYER_HPP

#include <QList>
#include "DataColor.hpp"
#include "FileFormat.hpp"

class CDataLayer
{

public: //types
    enum class WorkType{
        NOTHING,
        GENERATION,
        ENUMERATION
    };

public: //fields
    QString m_name;
    QString m_file_mask;
    QString m_path;
    bool m_one_file;
    double m_use_probability;
    double m_noise_probability;
    QList<CDataColor> m_colors;

public:
    CDataLayer();



};

#endif // CDATALAYER_HPP
