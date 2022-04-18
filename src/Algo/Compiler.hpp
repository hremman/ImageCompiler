#ifndef CPROCESSOR_HPP
#define CPROCESSOR_HPP

#include <random>
#include <QtConcurrent/QtConcurrent>
#include <QTemporaryFile>
#include <list>
#include <vector>
#include <map>
#include "Data/Project.hpp"
#include "ImageProcessing.hpp"
#include "SpinLock.hpp"

class CCompiler : public QObject
{
    Q_OBJECT
public:
    static CCompiler& getProcessor()
        {return __M_instance;}

    static bool Build(const Data::CProject &);
    static bool BuildOne(/*1 таска*/);

public:
    class Event
    {
    public:
        enum class Type
        {
            DONE,
            WARNING,
            ERROR,
        };

        enum class Stage
        {
            FIRST = 0,

            PREPARE = FIRST,
            GENERATE_PALETE,
            RANDOMISE_PREPARE,
            IMAGE_CHECK_AND_CAHCHE,
            IMAGE_COLOR_CACHE,
            TASK_BUILD,
            COMPILING,
            SAVE,

            COUNT,
            LAST = COUNT - 1
        };
    public:
        static const QString * stages();


    public:
        Event(Stage s, Type t, const QString & m)
            : m_stage(s)
            , m_type(t)
            , m_message(m)
        {}


        Type type() const
            { return m_type;}

        Stage stage() const
            { return m_stage;}

        const QString & message() const
            {return m_message;}

        const QString & typeDesc() const
            { return desc(m_type);}

        const QString & stageDesc() const
            { return desc(m_stage);}


    protected:
        Stage   m_stage;
        Type    m_type;
        QString m_message;

    protected:
        static const QString & desc(Type type);
        static const QString & desc(Stage stage);


    };

    class Context
    {

    public:
        class FileCache
        {
        protected:
            QTemporaryFile m_file;
            std::map<QString, qint64> m_index;

        public:
            FileCache()
                : m_file()
                , m_index()
            {}

            bool put(const QString & key, const QImage & img);

            QImage get(const QString &);

            bool isEmpty() const
                {return m_index.size() == 0;}

            void clear();


            ~FileCache()
            {
                if (m_file.isOpen())
                    m_file.close();
            }
        };

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

        std::map<QString, QImage*>                              m_file_to_image;
        std::map<QString, ImageCache>                           m_file_to_cache;

        std::map<QImage*, ImageCache>                           m_image_to_cache;
        std::map<Data::CLayer *, std::vector<Data::CColor>>     m_layer_to_paletes;

        FileCache m_file_cache;



        Context();

        void init_random(const Data::CProject &);
        void init_paletes(const Data::CProject &);
        CacheStatus build_files_cache(const Data::CProject &, QStringList &);
    private:
        std::mt19937 m_gen;
    };



public:
    bool build(const Data::CProject & proj, Context ** ctx_ptr = nullptr);
    bool buildOne(/*1 таска*/) {}

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


private:
    static CCompiler __M_instance;
    CCompiler();
    void wait();
    void work();

};

#endif // CPROCESSOR_HPP
