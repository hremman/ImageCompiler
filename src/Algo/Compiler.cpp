
#include "Exceptions.hpp"
#include "Compiler.hpp"


CCompiler CCompiler::__M_instance = CCompiler();

CCompiler::CCompiler()
    : QObject(nullptr)
    , m_signal(Signal::WORK)
    , m_lock()
{

}

bool CCompiler::Build(const Data::CProject & proj)
{return __M_instance.build(proj);}

bool CCompiler::BuildOne(/*1 таска*/)
    {{return __M_instance.buildOne(/*1 таска*/);}}

bool CCompiler::build(const Data::CProject & proj, Context ** ctx_ptr)
{
    if (m_signal != Signal::WAIT)
        throw CompilerBusy("Попытка запуска экземпляра компилятора, который занят");

    Context *ctx_ptr_local = nullptr;
    if (ctx_ptr != nullptr )
    {
        if ( *ctx_ptr == nullptr )
            *ctx_ptr = new Context();
        ctx_ptr_local = *ctx_ptr;
    }
    else
        ctx_ptr_local = new Context();
    Context & ctx = *ctx_ptr_local;

    ctx.init_random(proj);

    emit sendEvent(Event(Event::Stage::RANDOMISE_PREPARE, Event::Type::DONE, ""));

    while (m_signal != Signal::WORK)
    {
        switch (m_signal)
        {
            case Signal::PAUSE:
                while ( m_signal == Signal::PAUSE )
                    usleep(5);
                break;
            case Signal::STOP:
                if ( ctx_ptr == nullptr )
                    delete ctx_ptr_local;
                emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::ERROR, "Прервано пользователем"));
                m_lock.lock();
                m_signal = Signal::WAIT;
                m_lock.unlock();
                return false;
            case Signal::RESUME:
                m_lock.lock();
                m_signal = Signal::WORK;
                m_lock.unlock();
                break;
            case Signal::WORK:
            case Signal::WAIT:
                break;
        }
    }

    ctx.init_paletes(proj);

    emit sendEvent(Event(Event::Stage::GENERATE_PALETE, Event::Type::DONE, ""));


    while (m_signal != Signal::WORK)
    {
        switch (m_signal)
        {
            case Signal::PAUSE:
                while ( m_signal == Signal::PAUSE )
                    usleep(5);
                break;
            case Signal::STOP:
                if ( ctx_ptr == nullptr )
                    delete ctx_ptr_local;
                emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::ERROR, "Прервано пользователем"));
                m_lock.lock();
                m_signal = Signal::WAIT;
                m_lock.unlock();
                return false;
            case Signal::RESUME:
                m_lock.lock();
                m_signal = Signal::WORK;
                m_lock.unlock();
                break;
            case Signal::WORK:
            case Signal::WAIT:
                break;
        }
    }

    QStringList not_exists;
    CCompiler::Context::CacheStatus ret = ctx.build_files_cache(proj, not_exists);

    switch (ret)
    {
        case CCompiler::Context::CacheStatus::OK:
            emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::DONE, ""));
            break;
        case CCompiler::Context::CacheStatus::EMPTYSET:
            emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::ERROR, "Ни одного файла не подано на вход"));
            return false;
        case CCompiler::Context::CacheStatus::NOTEXISTS:
            emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::WARNING, "Переданные файлы не существуют или к ним нет доступа:\n" + not_exists.join("\n")));
            break;
        case CCompiler::Context::CacheStatus::ALLNOTEXISTS:
            emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::ERROR, "Все переданные файлы не существуют или к ним нет доступа:\n" + not_exists.join("\n")));
            return false;
    }

    while (m_signal != Signal::WORK)
    {
        switch (m_signal)
        {
            case Signal::PAUSE:
                while ( m_signal == Signal::PAUSE )
                    usleep(5);
                break;
            case Signal::STOP:
                if ( ctx_ptr == nullptr )
                    delete ctx_ptr_local;
                emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::ERROR, "Прервано пользователем"));
                m_lock.lock();
                m_signal = Signal::WAIT;
                m_lock.unlock();
                return false;
            case Signal::RESUME:
                m_lock.lock();
                m_signal = Signal::WORK;
                m_lock.unlock();
                break;
            case Signal::WORK:
            case Signal::WAIT:
                break;
        }
    }




    //add_line("Построение кэшей");
    //add_line("Подготовка заданий");
    //add_line("Генерация изображений");


}






void CCompiler::resume()
{
    m_lock.lock();
    m_signal = Signal::RESUME;
    m_lock.unlock();
}

void CCompiler::pause()
{
    m_lock.lock();
    m_signal = Signal::PAUSE;
    m_lock.unlock();
}

void CCompiler::stop()
{
    m_lock.lock();
    m_signal = Signal::STOP;
    m_lock.unlock();
}

void CCompiler::wait()
{
    m_lock.lock();
    m_signal = Signal::WAIT;
    m_lock.unlock();
}


void CCompiler::work()
{
    m_lock.lock();
    m_signal = Signal::WORK;
    m_lock.unlock();
}





const QString & CCompiler::Event::desc(Type type)
{
    static QString types[] = {
        "Выполнено",
        "Предупреждение",
        "Ошибка"
    };
    return types[static_cast<size_t>(type)];
}

const QString * CCompiler::Event::stages()
{
    static const QString stages[] = {
        "Подготовка контекста компиляции",
        "Подготовка генераторов случайности",
        "Генерация палитр",
        "Проверка и кэширование изображений",
        "Кэширование инфрормации о цветах",
        "Подготовка заданий",
        "Компиляция изображений",
        "Сохранение полученных изображений"
    };
    return stages;
}

const QString & CCompiler::Event::desc(Stage stage)
{
    return stages()[static_cast<size_t>(stage)];
}

CCompiler::Context::Context()
    : m_color_gen(0,255)
    , m_layer_to_noise()
    , m_layer_to_use()
    , m_file_to_image()
    , m_file_to_cache()
    , m_image_to_cache()
    , m_layer_to_paletes()
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
            m_file_cache.put(*file, img);
            ++num;
        }
    }
    return CacheStatus::OK | (not_exists.size() ? CacheStatus::NOTEXISTS : CacheStatus::OK) | (m_file_cache.isEmpty() ? CacheStatus::EMPTYSET : CacheStatus::OK);
}





QImage CCompiler::Context::FileCache::get(const QString & key)
{
    if ( m_index.find(key) == m_index.end() )
        throw NoSuchKey("Подан несуществующий ключ файла");

    m_file.seek(m_index.at(key));
    long long size = 0;
    m_file.read(reinterpret_cast<char *>(&size), sizeof (size));
    QByteArray data(size, Qt::Uninitialized);
    m_file.read(data.data(), size);
    QImage img;
    img.fromData(data);
    return img;
}

bool CCompiler::Context::FileCache::put(const QString & key, const QImage & img)
{
    if (!m_file.isOpen())
        m_file.open();
    if (m_index.find(key) == m_index.end())
    {
        QByteArray data(img.sizeInBytes(), Qt::Uninitialized);
        memcpy(data.data(), img.constBits(), img.sizeInBytes());
        m_file.seek(m_file.size());
        m_index[key] = m_file.pos();
        long long size = static_cast<long long>(data.size());
        m_file.write(reinterpret_cast<const char *>(&size), sizeof (size));
        m_file.write(data.data(), size);
        return true;
    }
    return false;
}

void CCompiler::Context::FileCache::clear()
{
    m_file.seek(0);
    m_file.resize(0);
    m_index.clear();
}
