#ifndef CPROCESSOR_HPP
#define CPROCESSOR_HPP

#include <random>
#include <QtConcurrent/QtConcurrent>
#include <QThreadPool>
#include <QTemporaryFile>
#include <list>
#include <vector>
#include <map>
#include "Data/Project.hpp"
#include "ImageProcessing.hpp"
#include "ImageStorage.hpp"
#include "SpinLock.hpp"
#include "Exceptions.hpp"


class CCompiler : public QObject
{
    Q_OBJECT
public:
    class Event;
    class Task;
    class Context;

public:
    static CCompiler& getProcessor()
        {return __M_instance;}

    static bool Build(const Data::CProject &);
    static bool BuildOne(Task &);

public:
    bool build(const Data::CProject & proj, Context ** ctx_ptr = nullptr);
    bool buildOne(Task &);

    void resume();
    void pause();
    void stop();

signals:
    void sendEvent(Event);

private:
    enum class Signal
    {
        WORK = 0,
        WAIT,
        RESUME,
        STOP,
        PAUSE
    };

    Signal m_signal;
    CSpinLock m_lock;
    bool eventLoop(Context **, Context *);
    QThreadPool m_pool;

private:
    static CCompiler __M_instance;
    CCompiler();
    void wait();
    void work();

};

#include "CompilerEvent.hpp"
#endif // CPROCESSOR_HPP
