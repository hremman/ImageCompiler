#ifndef TASK_HPP
#define TASK_HPP

#include "Compiler.hpp"

class CCompiler::Task
{
public:
    Task(CImageStorage * ptr);
    void resize(size_t);
    size_t size() const;

    void push(CImageStorage::iid_t, Data::CColor*);

    void clear();

protected:
    CImageStorage * m_store;
    std::vector<CImageStorage::iid_t> m_list;
    std::vector<Data::CColor*> m_trans;

    friend class CCompiler;
};

#endif // TASK_HPP
