#ifndef CPROCESSOR_HPP
#define CPROCESSOR_HPP


#include <QtConcurrent/QtConcurrent>
#include "Data/Project.hpp"

class CProcessor : public QObject
{
    Q_OBJECT
public:
    static CProcessor& getProcessor()
        {return __M_instance;}

    static bool Build(const Data::CProject &);
    static bool BuildOne(/*1 таска*/);

public:
    class Event
    {
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

            COUNT,
            LAST = COUNT - 1
        };

    protected:
        Type m_type;
        Stage m_stage;
        QString m_message;

    };


public:
    bool build(const Data::CProject &) const;
    bool buildOne(/*1 таска*/) const{}

signals:
    void sendEvent(Event);
private:
    size_t m_workers;

private:
    static CProcessor __M_instance;
    CProcessor();
};

#endif // CPROCESSOR_HPP
