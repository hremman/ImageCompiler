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
        QString m_name;
        QString m_out_path;
        QString m_file;


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
        CLayer * editable(const CLayer * );

        const std::list<CLayer*> & layers() const;
        const nlohmann::json to_json()  const;
        const nlohmann::json to_json(const QString &)  const;
        void from_json(const nlohmann::json & json);
        CProject& operator =(const CProject &);
        size_t count() const;
    protected:
        using uid_t = unsigned short;

    protected:
        std::set<uid_t> m_used_lid;
        std::list<CLayer*> m_layers;

    protected:
        uid_t new_lid();
        static const unsigned short __M_major_ver = 1;
        static const unsigned short __M_minor_ver = 0;
    };
}
#endif // CDATAPROJECT_HPP
