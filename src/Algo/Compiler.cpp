#include <QDir>
#include "Compiler.hpp"
#include "CompilerEvent.hpp"
#include "CompilerContext.hpp"
#include "CompilerTask.hpp"
#include "CompilerCursor.hpp"
#include "Exceptions.hpp"


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

bool CCompiler::BuildOne(TaskInterface *task, QImage &img)
    {{return __M_instance.buildOne(task, img);}}


bool CCompiler::buildOne(TaskInterface *task, QImage &img)
{return true;}



bool CCompiler::build(const Data::CProject & proj, Context ** ctx_ptr)
{

    if (m_signal != Signal::WAIT)
    {
        emit sendEvent(Event::Error(Event::Stage::PREPARE, "Попытка запуска экземпляра компилятора, который занят"));
        return false;
    }

    emit sendEvent(Event::Startted(Event::Stage::PREPARE));

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

    emit sendEvent(Event::Ok(Event::Stage::PREPARE));

    emit sendEvent(Event::Startted(Event::Stage::IMAGE_CHECK_AND_CAHCHE));

    {
        QStringList not_exists;
        try
        {
            CCompiler::Context::CacheStatus ret = ctx.build_files_cache(proj, not_exists, [this] (size_t t, size_t d) {emit this->sendEvent(Event::Progress(Event::Stage::RANDOMISE_PREPARE, std::make_pair(t,d)));});

            switch (ret)
            {
                case CCompiler::Context::CacheStatus::OK:
                    emit sendEvent(Event::Ok(Event::Stage::IMAGE_CHECK_AND_CAHCHE));
                    break;
                case CCompiler::Context::CacheStatus::EMPTYSET:
                    emit sendEvent(Event::Error(Event::Stage::IMAGE_CHECK_AND_CAHCHE, "Ни одного файла не подано на вход"));
                    return false;
                case CCompiler::Context::CacheStatus::NOTEXISTS:
                    emit sendEvent(Event::Warn(Event::Stage::IMAGE_CHECK_AND_CAHCHE, "Переданные файлы не существуют или к ним нет доступа:\n" + not_exists.join("\n")));
                    break;
                case CCompiler::Context::CacheStatus::ALLNOTEXISTS:
                    emit sendEvent(Event::Error(Event::Stage::IMAGE_CHECK_AND_CAHCHE, "Все переданные файлы не существуют или к ним нет доступа:\n" + not_exists.join("\n")));
                    return false;
            }

        }
        catch (const ImageStorage &e)
        {
            emit sendEvent(Event::Error(Event::Stage::IMAGE_CHECK_AND_CAHCHE, "Ошибка построения кэша изображений ("+e.message()+")"));
            return false;
        }
    }
    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;

    emit sendEvent(Event::Startted(Event::Stage::RANDOMISE_PREPARE));
    emit sendEvent(Event::Progress(Event::Stage::RANDOMISE_PREPARE, std::make_pair(0,0)));

    ctx.init_random();

    emit sendEvent(Event::Ok(Event::Stage::RANDOMISE_PREPARE));
    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;


    emit sendEvent(Event::Startted(Event::Stage::GENERATE_PALETE));

    ctx.init_paletes();

    emit sendEvent(Event::Ok(Event::Stage::GENERATE_PALETE));
    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;

    emit sendEvent(Event::Startted(Event::Stage::IMAGE_COLOR_CACHE));
    {
        size_t done = 0;
        for (auto files = ctx.m_layer_to_file.begin(); files != ctx.m_layer_to_file.end(); ++files)
            for (auto it = files->second.begin(); it != files->second.end(); it++)
            {
                ++done;
                QImage img = ctx.m_file_cache.get(*it);
                std::vector<ImageCache> cache;
                cache.resize(m_pool.maxThreadCount());
                QFutureWatcher<bool> * watchdog = new QFutureWatcher<bool>[m_pool.maxThreadCount()] ;

                for (size_t i = 0 ; i < m_pool.maxThreadCount(); i++)
                {
                    //watchdog.emplace_back(nullptr);
                    watchdog[i].setFuture(QtConcurrent::run(&m_pool,[&]() -> bool { return CImageProcessing::getAverage(img, static_cast<size_t>(0), static_cast<size_t>(m_pool.maxThreadCount()), cache[i].m_saturation, cache[i].m_value);}));
                }
                for (size_t i = 0 ; i < m_pool.maxThreadCount(); i++)
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

                delete [] watchdog;
                if (done % 10 == 0)
                    emit sendEvent(Event::Progress(Event::Stage::IMAGE_COLOR_CACHE, std::make_pair(ctx.m_file_cache.count(),done)));

                if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;
            }

    }

    emit sendEvent(Event(Event::Stage::IMAGE_COLOR_CACHE));
    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;

    emit sendEvent(Event::Startted(Event::Stage::COMPILING));
    CImageStorage out_storage(CImageStorage::GB);
    QString error;
    {
        emit sendEvent(Event::Progress(Event::Stage::COMPILING, std::make_pair(0,0)));
        Cursor cur;
        cur.m_colors.resize(ctx.m_layers.size());
        cur.m_files.resize(ctx.m_layers.size());
        cur.m_current_color.resize(ctx.m_layers.size());
        cur.m_current_file.resize(ctx.m_layers.size());
        auto layer = ctx.m_layers.begin();
        for (size_t it = 0; it < cur.m_current_color.size() && layer != ctx.m_layers.end(); ++it, ++layer)
        {
            cur.m_current_file[it] = 0;
            cur.m_files[it].insert(cur.m_files[it].begin(), ctx.m_layer_to_file[*layer].begin(), ctx.m_layer_to_file[*layer].end());
            if(ctx.m_layer_to_paletes.find(*layer) != ctx.m_layer_to_paletes.end())
                cur.m_colors[it] = &ctx.m_layer_to_paletes[*layer];
            else
                cur.m_colors[it] = &((*layer)->m_colors.m_colors);
            cur.m_current_color[it] = cur.m_colors[it]->begin();
        }

        Cursor::Sequence seq;
        std::set<Data::CLayer*> noised;
        size_t total_num = proj.count();
        size_t progres_ever = total_num > 100 ? total_num / 100 : 1;
        size_t counter = 0;

        try
        {
            while (cur.get_next(seq))
            {
                counter++;
                layer = ctx.m_layers.begin();
                TaskStorage task(ctx.m_file_cache);
                TaskStorage taskNoised(ctx.m_file_cache);
                bool noise = false;
                for (size_t it = 0; it < seq.size() && layer != ctx.m_layers.end(); ++it, ++layer)
                {
                    if ( (*layer)->m_blink )
                    {
                        if ( ctx.noise(*layer) && noised.find(*layer) == noised.end() )
                        {
                            noise = true;
                            taskNoised.push(seq[it].first, reinterpret_cast<Data::CColor *>(1), *(ctx.m_id_to_cache[seq[it].first]));
                            noised.insert(*layer);
                        }
                        task.push(seq[it].first, seq[it].second, *(ctx.m_id_to_cache[seq[it].first]));
                        taskNoised.push(seq[it].first, seq[it].second, *(ctx.m_id_to_cache[seq[it].first]));

                    }
                    else
                        if ( ctx.usage(*layer) )
                        {
                            if ( ctx.noise(*layer) && noised.find(*layer) == noised.end() )
                            {
                                noise = true;
                                taskNoised.push(seq[it].first, reinterpret_cast<Data::CColor *>(1), *(ctx.m_id_to_cache[seq[it].first]));
                                noised.insert(*layer);
                            }
                            task.push(seq[it].first, seq[it].second, *(ctx.m_id_to_cache[seq[it].first]));
                            taskNoised.push(seq[it].first, seq[it].second, *(ctx.m_id_to_cache[seq[it].first]));
                        }
                }


                    QImage img;
                    if ( buildOne(&task, img) )
                        out_storage.put(img);
                    if ( noise )
                        if ( buildOne(&taskNoised, img) )
                            out_storage.put(img);


                if ( counter % progres_ever == 0)
                    emit sendEvent(Event::Progress(Event::Stage::COMPILING, std::make_pair(total_num, counter)));
                if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;
            }
        }
        catch (const ImageStorage &e)
            { error = "Ошибка построения кэша изображений ("+e.message()+")"; }

    }
    emit sendEvent(error.isEmpty()? Event::Ok(Event::Stage::COMPILING) : Event::Error(Event::Stage::COMPILING, error));
    if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;
    emit sendEvent(Event::Progress(Event::Stage::SAVE, std::make_pair(0, 0)));
    emit sendEvent(Event::Startted(Event::Stage::SAVE));
    {

        if ( ! QDir(proj.m_out_path).exists() )
            if ( !QDir().mkpath(proj.m_out_path) )
            {
                emit sendEvent(Event::Error(Event::Stage::SAVE, "Не удалось создать папку: " + proj.m_out_path));
                return false;
            }
        size_t total_num = out_storage.count();
        int number_lenght = QString::number(total_num).length();
        size_t progres_ever = total_num > 100 ? total_num / 100 : 1;
        size_t counter = 0;
        size_t success = 0;
        for (auto iid_it = out_storage.begin(); iid_it != out_storage.end(); ++iid_it)
        {
            counter++;
            QImage img = out_storage.getCopy(*iid_it);
            success += img.save(proj.m_out_path +( proj.m_out_path[proj.m_out_path.size() -1 ] != '/' ? "" : "/" ) + QString::number(*iid_it).rightJustified(number_lenght, '0') + ".png", "PNG" ) ? 1 : 0;

            if ( counter % progres_ever == 0)
                emit sendEvent(Event::Progress(Event::Stage::SAVE, std::make_pair(total_num, counter)));
            if ( !eventLoop(ctx_ptr, ctx_ptr_local) ) return false;
        }
        emit sendEvent(success == total_num ?
                           Event::Ok(Event::Stage::SAVE) :
                           ( success != 0 ?
                                Event::Warn(Event::Stage::COMPILING, "Не удалось записать на диск " + QString::number(total_num-success) + " изображений") :
                                Event::Warn(Event::Stage::COMPILING, "Не удалось записать на диск ни одного изображения ") ));
    }
    return true;
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

