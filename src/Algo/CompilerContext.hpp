#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Compiler.hpp"



class CCompiler::Context
{
public:
    std::uniform_int_distribution<unsigned char>            m_color_gen;
    std::map<Data::CLayer *, std::bernoulli_distribution>   m_layer_to_noise;
    std::map<Data::CLayer *, std::bernoulli_distribution>   m_layer_to_use;

    std::map<CImageStorage::iid_t, std::list<ImageCache>::iterator> m_id_to_cache;

    std::map<Data::CLayer *, std::list<Data::CColor>>     m_layer_to_paletes;
    std::list<ImageCache> m_caches;

    std::map<Data::CLayer *, std::list<CImageStorage::iid_t>>     m_layer_to_file;
    std::list<Data::CLayer*> m_layers;


    enum class CacheStatus : char
    {
        OK = 0,
        NOTEXISTS = 1,
        EMPTYSET = 2,
        ALLNOTEXISTS = NOTEXISTS | EMPTYSET

    };

    Context(long long limit = 1 * CImageStorage::GB);
    Context(CImageStorage &);

    void init_random();
    void init_paletes();

    bool noise(Data::CLayer*);
    bool usage(Data::CLayer*);


    CacheStatus build_files_cache(const Data::CProject &, QStringList &);

    template <class FUNC>
    CacheStatus build_files_cache(const Data::CProject &proj, QStringList & not_exists, FUNC onProgress, size_t every = 10)
    {
        not_exists.clear();
        auto layers = proj.layers();
        size_t tnum = 0;
        size_t num = 0;
        size_t precount = 0;
        for (auto it = layers.begin(); it != layers.end(); it++)
            precount += (*it)->m_files.size();

        for (auto it = layers.begin(); it != layers.end(); it++)
        {
            for (auto file = (*it)->m_files.begin(); file != (*it)->m_files.end(); file++ )
            {
                ++tnum;
                if (tnum % every == 0)
                    onProgress(precount, tnum);
                if ( ! QFile::exists(*file) )
                {
                    not_exists.append(*file);
                    continue;
                }
                QImage img(*file);

                if (m_layers.begin() == m_layers.end() || m_layers.back() != *it)
                    m_layers.push_back(*it);
                m_layer_to_file[*it].push_back(m_file_cache.put(*file, img));

                ++num;
            }
        }

        QImage empty(2,2,m_file_cache.get(*(m_file_cache.begin())).format());
        empty.setPixelColor(0,0, QColor(0,0,0,0));
        empty.setPixelColor(0,1, QColor(0,0,0,0));
        empty.setPixelColor(1,0, QColor(0,0,0,0));
        empty.setPixelColor(1,1, QColor(0,0,0,0));
        auto empty_iid = m_file_cache.put(empty);
        for (auto it = m_layers.begin(); it != m_layers.end(); it++)
        {
            if ( (*it)->m_blink )
                m_layer_to_file[*it].push_back(empty_iid);
        }
        return static_cast <CacheStatus>(static_cast <char>(CacheStatus::OK) | (not_exists.size() ? static_cast <char>(CacheStatus::NOTEXISTS) : static_cast <char>(CacheStatus::OK)) | (m_file_cache.isEmpty() ? static_cast <char>(CacheStatus::EMPTYSET) : static_cast <char>(CacheStatus::OK)));
    }


private:
    CImageStorage m_file_cache_private;
public:
    CImageStorage &m_file_cache;
private:
    std::mt19937 m_gen;
};


#endif // CONTEXT_HPP
