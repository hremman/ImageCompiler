#ifndef CDATAPROJECT_HPP
#define CDATAPROJECT_HPP

#include <QString>
#include <list>
#include <map>
#include "Data/Layer.hpp"




namespace Data {
    class CProject
    {
    public:
        class State
        {
        public:
            std::list<CLayer*>::const_iterator m_current_layer;
            size_t m_current_file;
            std::list<CColor>::const_iterator m_current_color;
        };

    public:
        QString m_name;
        QString m_out_path;
        QString m_file;


    public:
        CProject();
        CProject(const CProject &);
        CLayer * newLayer();
        void removeLayer(CLayer *);
        void moveLayerUp(size_t);
        void moveLayerDown(size_t);

        const std::list<CLayer*> & layers() const;
        const nlohmann::json to_json()  const;
        void from_jsom(const nlohmann::json & json);
        CProject& operator =(const CProject &);
        size_t count() const;
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
