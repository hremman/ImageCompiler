#ifndef EVENT_HPP
#define EVENT_HPP
#include "Compiler.hpp"

class CCompiler::Event
{
public:
    enum class Type
    {
        OK,
        INWORK,
        WARNING,
        ERROR,
        ONPROGRESS
    };

    enum class Stage
    {
        FIRST = 0,

        PREPARE = FIRST,
        IMAGE_CHECK_AND_CAHCHE,
        GENERATE_PALETE,
        RANDOMISE_PREPARE,
        IMAGE_COLOR_CACHE,
        COMPILING,
        SAVE,

        COUNT,
        LAST = COUNT - 1
    };
public:
    static const QString * stages();

    static Event Progress(Stage s, std::pair<size_t, size_t> d)
        { return Event(s,d); }
    static Event Ok(Stage s)
        { return Event(s); }
    static Event Startted(Stage s)
        { return Event(s); }
    static Event Ok(Stage s, const QString &str)
        { return Event(s, Type::OK, str); }
    static Event Error(Stage s, const QString &str)
        { return Event(s, Type::ERROR, str); }
    static Event Warn(Stage s, const QString &str)
        { return Event(s, Type::WARNING, str); }


public:
    Event(Stage s, Type t, const QString & m)
        : m_stage(s)
        , m_type(t)
        , m_data(nullptr)
    {
        if ( t == Type::ONPROGRESS )
            WrongType("Некорректный тип события \"Прогресс\"");
        m_data = new QString(m);
    }

    Event(Stage s, std::pair<size_t, size_t> d)
        : m_stage(s)
        , m_type(Type::ONPROGRESS)
        , m_data(nullptr)
    {
        m_data = new std::pair<size_t, size_t>(d);
    }

    Event(Stage s)
        : m_stage(s)
        , m_type( Type::OK )
        , m_data(nullptr)
    {}


    ~Event()
    {
        if ( m_data != nullptr )
        {
            if ( m_type == Type::ONPROGRESS )
                delete reinterpret_cast<std::pair<size_t, size_t>*>( m_data );
            else
                delete reinterpret_cast<QString*>( m_data );
        }
    }


    Type type() const
        { return m_type;}

    Stage stage() const
        { return m_stage;}

    const QString & message() const
        { return m_data != nullptr ? *(reinterpret_cast<QString*>(m_data)) : ""; }

    std::pair<size_t, size_t> progress() const
        { return m_data != nullptr ? *(reinterpret_cast<std::pair<size_t, size_t>*>(m_data)) : std::make_pair<size_t, size_t>(0,0); }

    const void* raw() const
        { return m_data; }

    const QString & typeDesc() const
        { return desc(m_type);}

    const QString & stageDesc() const
        { return desc(m_stage);}


protected:
    Stage   m_stage;
    Type    m_type;
    void *  m_data;

protected:
    static const QString & desc(Type type);
    static const QString & desc(Stage stage);

};

#endif // EVENT_HPP
