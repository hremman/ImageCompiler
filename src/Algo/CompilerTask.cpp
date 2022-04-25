#include "CompilerTask.hpp"

CCompiler::Task::Task()
    : m_images()
    , m_trans()
{}

void CCompiler::Task::resize(size_t s)
{
    m_images.resize(s);
    m_trans.resize(s);
}

size_t CCompiler::Task::size() const
    { return m_trans.size(); }


void CCompiler::Task::push(const QImage & i, Data::CColor* c)
{
    m_trans.push_back(c);
    m_images.push_back(i);
}

void CCompiler::Task::clear()
{
    m_trans.clear();
    m_images.clear();
}
