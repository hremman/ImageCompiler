#ifndef CDATAPROJECT_HPP
#define CDATAPROJECT_HPP

#include <QString>
#include <list>
#include "Layer.hpp"

namespace Data {
    class CProject
    {
    public:
        QString m_name;
        QString m_out_path;
        QString m_root_path;
        std::list<CLayer> m_layers;


    public:
        CProject();
        CProject(const CProject &);

        CProject& operator =(const CProject &);
    };
}
#endif // CDATAPROJECT_HPP
