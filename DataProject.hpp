#ifndef CDATAPROJECT_HPP
#define CDATAPROJECT_HPP

#include <QString>
#include <QList>
#include "DataLayer.hpp"

class CDataProject
{
public:
    QString m_name;
    QString m_out_path;
    QString m_root_path;
    QList<CDataLayer> m_layers;


public:
    CDataProject();
};

#endif // CDATAPROJECT_HPP
