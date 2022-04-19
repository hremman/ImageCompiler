#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "Compiler.hpp"

class CCompiler::Context
{
public:
    enum class CacheStatus : char
    {
        OK = 0,
        NOTEXISTS = 1,
        EMPTYSET = 2,
        ALLNOTEXISTS = NOTEXISTS | EMPTYSET
    };

public:
    std::uniform_int_distribution<unsigned char>            m_color_gen;
    std::map<Data::CLayer *, std::bernoulli_distribution>   m_layer_to_noise;
    std::map<Data::CLayer *, std::bernoulli_distribution>   m_layer_to_use;

    std::map<CImageStorage::iid_t, std::list<ImageCache>::iterator> m_id_to_cache;

    std::map<Data::CLayer *, std::vector<Data::CColor>>     m_layer_to_paletes;
    std::list<ImageCache> m_caches;


    std::list<CImageStorage::iid_t> m_files;




    Context();
    Context(CImageStorage &);

    void init_random(const Data::CProject &);
    void init_paletes(const Data::CProject &);
    CacheStatus build_files_cache(const Data::CProject &, QStringList &);

    template <class FUNC>
    CacheStatus build_files_cache(const Data::CProject &, QStringList &, FUNC, size_t every = 10);
private:
    CImageStorage m_file_cache_private;
public:
    CImageStorage &m_file_cache;
private:
    std::mt19937 m_gen;
};

#endif // CONTEXT_HPP
