#ifndef TASK_HPP
#define TASK_HPP

#include "Compiler.hpp"

class CCompiler::Task
{
public:
    Task();

    size_t size() const;

    void push(const QImage &, Data::CColor*);
    void clear();

protected:
    void resize(size_t);
    std::vector<QImage> m_images;
    std::vector<Data::CColor*> m_trans;

    friend class CCompiler;
};

#endif // TASK_HPP
