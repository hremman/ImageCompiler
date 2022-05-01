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
    class TaskInterface;
    class Context;


public:
    static CCompiler& getGlobal()
        {return __M_instance;}

    static bool Build(const Data::CProject &);
    static bool BuildOne(TaskInterface *, QImage &);

public:
    CCompiler();
    bool build(const Data::CProject & proj, Context ** ctx_ptr = nullptr);


    void resume();
    void pause();
    void stop();
    bool isFree() const;

signals:
    void sendEvent(Event);

private:
    class Cursor;
    enum class Signal
    {
        WORK = 0,
        WAIT,
        RESUME,
        STOP,
        PAUSE
    };

private:
    Signal m_signal;
    CSpinLock m_lock;
    bool eventLoop(Context **, Context *);
    QThreadPool m_pool;

private:
    static CCompiler __M_instance;
    bool buildOne(TaskInterface *, QImage &);
    void wait();
    void work();
    void transform(void *);

};

#include "CompilerEvent.hpp"
#endif // CPROCESSOR_HPP
