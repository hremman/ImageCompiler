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
        using CChain = std::map<const CLayer* const, CLayer::CState>;

    public:
        QString m_name;
        QString m_out_path;
        QString m_file;
        CChain *m_current_chain;


    public:
        CProject();
        ~CProject();
        CProject(const CProject &);
        CLayer * newLayer();
        void removeLayer(CLayer *);
        void moveLayerUp(size_t);
        void moveLayerDown(size_t);
        bool hasChain() const;
        void useChain(bool);

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
