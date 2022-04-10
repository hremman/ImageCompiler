#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <QString>

class ICException : public std::runtime_error
{
public:
    ICException(const QString & msg)
        : std::runtime_error(msg.toStdString())
        , m_msg(msg)
    {}
    virtual QString message() const
    {
        return m_msg;
    }

protected:
    QString m_msg;
};

class wrong_mode : public ICException
{
public:
    wrong_mode(const QString & msg) : ICException(msg) {}
};

class too_many_id : public ICException
{
public:
    too_many_id(const QString & msg) : ICException(msg) {}
};

class story_out_of_range: public ICException
{
public:
    story_out_of_range(const QString & msg) : ICException(msg) {}
};

class nullpointer : public ICException
{
public:
    nullpointer(const QString & msg) : ICException(msg) {}
};

class wrong_arg : public ICException
{
public:
    wrong_arg(const QString & msg) : ICException(msg) {}
};

#endif // EXCEPTIONS_HPP
