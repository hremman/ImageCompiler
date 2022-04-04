#ifndef CDATAPROJECT_HPP
#define CDATAPROJECT_HPP

#include <QString>
#include <list>
#include <map>
#include "Layer.hpp"

namespace Data {
    class CProject
    {

    public:
        QString m_name;
        QString m_out_path;
        QString m_root_path;



    public:
        CProject();
        CProject(const CProject &);
        CLayer * newLayer();
        void removeLayer(CLayer *);
        void moveLayerUp(size_t);
        void moveLayerDown(size_t);

        CProject& operator =(const CProject &);
    protected:
        using uid_t = unsigned short;

    protected:
        std::set<uid_t> m_used_lid;
        std::list<CLayer*> m_layers;

    protected:
        uid_t new_lid();
    };
}
#endif // CDATAPROJECT_HPP
