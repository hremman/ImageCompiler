#ifndef CPROCESSOR_HPP
#define CPROCESSOR_HPP


#include <QtConcurrent/QtConcurrent>
#include "Data/Project.hpp"

class CProcessor
{
public:
    static CProcessor& getProcessor()
        {return __M_instance;}

    static bool Build(const Data::CProject &);
    static bool BuildOne(/*1 таска*/);



public:
    bool build(const Data::CProject &) const;
    bool buildOne(/*1 таска*/) const;


private:
    size_t m_workers;

private:
    static CProcessor __M_instance;
    CProcessor();
};

#endif // CPROCESSOR_HPP
