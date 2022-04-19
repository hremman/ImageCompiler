#ifndef EVENT_HPP
#define EVENT_HPP
#include "Compiler.hpp"

class CCompiler::Event
{
public:
    enum class Type
    {
        OK,
        WARNING,
        ERROR,
        ONPROGRESS
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
        , m_data(nullptr)
    {
        if ( t == Type::ONPROGRESS )
            WrongType("Некорректный тип события \"Прогресс\"");
        m_data = new QString(m);
    }

    Event(Stage s, Type t, std::pair<size_t, size_t> d)
        : m_stage(s)
        , m_type(t)
        , m_data(nullptr)
    {
        if ( t != Type::ONPROGRESS )
            WrongType("Некорректный тип события " + desc(t));
        m_data = new std::pair<size_t, size_t>(d);
    }

    Event(Stage s, Type t)
        : m_stage(s)
        , m_type(t)
        , m_data(nullptr)
    {
        if ( t != Type::OK )
            WrongType("Некорректный тип события " + desc(t));
    }


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
