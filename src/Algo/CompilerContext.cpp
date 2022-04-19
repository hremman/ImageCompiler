#include "CompilerContext.hpp"


CCompiler::Context::Context()
    : m_color_gen(0,255)
    , m_layer_to_noise()
    , m_layer_to_use()
    , m_layer_to_paletes()
    , m_file_cache_private(2*CImageStorage::GB)
    , m_file_cache(m_file_cache_private)
    , m_gen(std::random_device()())
{}

CCompiler::Context::Context(CImageStorage &storage)
    : m_color_gen(0,255)
    , m_layer_to_noise()
    , m_layer_to_use()
    , m_layer_to_paletes()
    , m_file_cache_private(1*CImageStorage::B)
    , m_file_cache(storage)
    , m_gen(std::random_device()())
{}

void CCompiler::Context::init_random(const Data::CProject & proj)
{
    auto layers = proj.layers();

    //add_line("Подготовка случайных генераторов");

    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        if ( (*it)->m_noise_probability != 0.0 )
        {
            m_layer_to_noise[*it] = std::bernoulli_distribution((*it)->m_use_probability < 1.0 ? (*it)->m_use_probability : 1);
        }

        if ( (*it)->m_use_probability != 0.0 )
        {
            m_layer_to_use[*it] = std::bernoulli_distribution((*it)->m_use_probability < 1.0 ? (*it)->m_use_probability: 1 );
        }
    }
}


void CCompiler::Context::init_paletes(const Data::CProject & proj)
{
    auto layers = proj.layers();
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        if ( (*it)->m_type == Data::CLayer::WorkType::ENUMERATION )
        {
            if ((*it)->m_colors.m_mode == Data::CColorSettings::Mode::GENERATION)
            {
                m_layer_to_paletes[*it] = std::vector<Data::CColor>();
                m_layer_to_paletes[*it].resize((*it)->m_colors.m_generations_number);
                for (size_t i = 0; i < m_layer_to_paletes[*it].size(); i++)
                    m_layer_to_paletes[*it][i] = Data::CColor(m_color_gen(m_gen), m_color_gen(m_gen), m_color_gen(m_gen) );
            }
        }
    }
}


CCompiler::Context::CacheStatus operator|(CCompiler::Context::CacheStatus l, CCompiler::Context::CacheStatus r)
    { return static_cast<CCompiler::Context::CacheStatus>(static_cast <char>(l) | static_cast <char>(r)); }

CCompiler::Context::CacheStatus CCompiler::Context::build_files_cache(const Data::CProject & proj, QStringList &not_exists)
{
    not_exists.clear();
    auto layers = proj.layers();
    size_t tnum = 0;
    size_t num = 0;
    for (auto it = layers.begin(); it != layers.end(); it++)
    {
        for (auto file = (*it)->m_files.begin(); file != (*it)->m_files.end(); file++ )
        {
            ++tnum;
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
    return CacheStatus::OK | (not_exists.size() ? CacheStatus::NOTEXISTS : CacheStatus::OK) | (m_file_cache.isEmpty() ? CacheStatus::EMPTYSET : CacheStatus::OK);
}

template <class FUNC>
CCompiler::Context::CacheStatus CCompiler::Context::build_files_cache(const Data::CProject & proj, QStringList &not_exists, FUNC onProgress, size_t every)
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
            auto id = m_file_cache.put(*file, img);
            ++num;
        }
    }
    return CacheStatus::OK | (not_exists.size() ? CacheStatus::NOTEXISTS : CacheStatus::OK) | (m_file_cache.isEmpty() ? CacheStatus::EMPTYSET : CacheStatus::OK);
}
