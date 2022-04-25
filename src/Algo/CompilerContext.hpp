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


    std::list<CImageStorage::iid_t> m_files;


    enum class CacheStatus : char
    {
        OK = 0,
        NOTEXISTS = 1,
        EMPTYSET = 2,
        ALLNOTEXISTS = NOTEXISTS | EMPTYSET

    };

    Context(long long limit = 1 * CImageStorage::GB);
    Context(CImageStorage &);

    void init_random(const Data::CProject &);
    void init_paletes(const Data::CProject &);


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
                m_files.push_back(m_file_cache.put(*file, img));
                ++num;
            }
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
