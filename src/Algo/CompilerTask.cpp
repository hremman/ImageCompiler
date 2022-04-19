#include "CompilerTask.hpp"

CCompiler::Task::Task(CCompiler::ImageStore * ptr)
    : m_cache(ptr)
{}

void CCompiler::Task::resize(size_t s)
{
    m_list.resize(s);
    m_trans.resize(s);
}

size_t CCompiler::Task::size() const
    { return m_trans.size(); }

void CCompiler::Task::push(CImageStorage::iid_t id, Data::CColor* c)
{
    m_trans.push_back(c);
    m_list.push_back(id);
}

void CCompiler::Task::clear()
{
    m_trans.clear();
    m_list.clear();
}
