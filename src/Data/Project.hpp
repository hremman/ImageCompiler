#ifndef CDATAPROJECT_HPP
#define CDATAPROJECT_HPP

#include <QString>
#include <QList>
#include "DLayer.hpp"

namespace Data {
    class CProject
    {
    public:
        QString m_name;
        QString m_out_path;
        QString m_root_path;
        QList<CLayer> m_layers;


    public:
        CProject();
    };
}
#endif // CDATAPROJECT_HPP
