
#include "Exceptions.hpp"
#include "Compiler.hpp"
#include "CompilerEvent.hpp"
#include "CompilerContext.hpp"
#include "CompilerTask.hpp"


CCompiler CCompiler::__M_instance = CCompiler();

CCompiler::CCompiler()
    : QObject(nullptr)
    , m_signal(Signal::WORK)
    , m_lock()
    , m_pool()
{
    int threads = QThread::idealThreadCount();
    if ( threads  <= 4)
        threads = 2;
    else
        threads -= 3;
    m_pool.setMaxThreadCount(threads);
}

bool CCompiler::Build(const Data::CProject & proj)
{return __M_instance.build(proj);}

bool CCompiler::BuildOne(Task &task)
    {{return __M_instance.buildOne(task);}}


bool CCompiler::buildOne(Task &task)
{return true;}

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

    emit sendEvent(Event(Event::Stage::PREPARE, Event::Type::OK));

    ctx.init_random(proj);

    emit sendEvent(Event(Event::Stage::RANDOMISE_PREPARE, Event::Type::OK));

   if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;



    ctx.init_paletes(proj);

    emit sendEvent(Event(Event::Stage::GENERATE_PALETE, Event::Type::OK));


    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;



    {
        QStringList not_exists;
        CCompiler::Context::CacheStatus ret = ctx.build_files_cache(proj, not_exists, [this] (size_t t, size_t d) {emit this->sendEvent(Event(Event::Stage::RANDOMISE_PREPARE, Event::Type::OK, std::make_pair(t,d)));});

        switch (ret)
        {
            case CCompiler::Context::CacheStatus::OK:
                emit sendEvent(Event(Event::Stage::IMAGE_CHECK_AND_CAHCHE, Event::Type::OK));
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
    }

    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;

    {
        auto layers = proj.layers();
        size_t done = 0;
        for (auto it = ctx.m_files.begin(); it != ctx.m_files.end(); it++)
        {
            ++done;
            QImage img = ctx.m_file_cache.get(*it);
            std::vector<ImageCache> cache;
            cache.resize(m_pool.maxThreadCount());
            std::vector<QFutureWatcher<void>> watchdog;
            watchdog.resize(m_pool.maxThreadCount());

            for (size_t i = 0 ; i < watchdog.size(); i++)
                watchdog[i].setFuture(QtConcurrent::run(&m_pool, CImageProcessing::getAverage<true, true>, img, 0, m_pool.maxThreadCount(), cache[i].m_saturation, cache[i].m_value));
            for (size_t i = 0 ; i < watchdog.size(); i++)
                watchdog[i].waitForFinished();

            ImageCache fin;

            for (size_t i = 0 ; i < cache.size(); i++)
            {
                if ( fin.m_saturation.m_down > cache[i].m_saturation.m_down )
                    fin.m_saturation.m_down = cache[i].m_saturation.m_down;
                if ( fin.m_value.m_down > cache[i].m_value.m_down )
                    fin.m_value.m_down = cache[i].m_value.m_down;
                if ( fin.m_saturation.m_top < cache[i].m_saturation.m_top )
                    fin.m_saturation.m_top = cache[i].m_saturation.m_top;
                if ( fin.m_value.m_top > cache[i].m_value.m_top )
                    fin.m_value.m_top = cache[i].m_value.m_top;
                fin.m_saturation.m_average += cache[i].m_saturation.m_average;
                fin.m_value.m_average += cache[i].m_value.m_average;
            }
            fin.m_saturation.m_average /= cache.size();
            fin.m_value.m_average /= cache.size();


            ctx.m_caches.push_back(fin);
            ctx.m_id_to_cache[*it] = --ctx.m_caches.end();


            if (done % 10 == 0)
                emit sendEvent(Event(Event::Stage::RANDOMISE_PREPARE, Event::Type::OK, std::make_pair(ctx.m_file_cache.count(),done)));
        }

    }

    emit sendEvent(Event(Event::Stage::IMAGE_COLOR_CACHE, Event::Type::OK));
    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;

    //add_line("Подготовка заданий");

    std::list<Task> tasks;
    auto layers = proj.layers();




    //add_line("Генерация изображений");


}

bool CCompiler::eventLoop(Context ** ctx_ptr, Context *ctx_ptr_local)
{
    while (m_signal != Signal::WORK)
    {
        switch (m_signal)
        {
            case Signal::PAUSE:
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
                return true;
            case Signal::WORK:
            case Signal::WAIT:
                return true;
        }
    }
    return true;
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

