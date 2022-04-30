#ifndef TASK_HPP
#define TASK_HPP

#include "Compiler.hpp"

class CCompiler::TaskInterface
{
public:
    struct LayerInfo
    {
        const QImage& img;
        Data::CColor* color;
        const ImageCache & cache;
    };
public:
    TaskInterface()
        : m_trans()
        , m_cache()
    {}
    TaskInterface(const TaskInterface &ref)
        : m_trans(ref.m_trans)
        , m_cache(ref.m_cache)
    {}

    virtual ~TaskInterface()
    {}

    virtual LayerInfo at(size_t) = 0;
    size_t size() const
        { return m_trans.size(); }
    virtual void clear() = 0;
protected:
    std::vector<Data::CColor*> m_trans;
    std::vector<ImageCache> m_cache;
};


class TaskStorage : public CCompiler::TaskInterface
{
public:
    TaskStorage(CImageStorage &);
    TaskStorage(const TaskStorage &);

    void push(CImageStorage::iid_t, Data::CColor*, const ImageCache &);

    virtual void clear() override;
    virtual LayerInfo at(size_t) override;

    virtual ~TaskStorage()
    {}

protected:
    std::vector<CImageStorage::iid_t> m_images;
    CImageStorage & m_storage;
};

class Task : public CCompiler::TaskInterface
{
public:
    Task();

    void push(const QImage &, Data::CColor*, const ImageCache &);

    virtual void clear() override;
    virtual LayerInfo at(size_t) override;

    virtual ~Task()
    {}
protected:
    std::vector<QImage> m_images;

};

#endif // TASK_HPP
