#include "CompilerTask.hpp"


TaskStorage::TaskStorage(CImageStorage &ref)
    : CCompiler::TaskInterface()
    , m_images()
    , m_storage(ref)
{
}

TaskStorage::TaskStorage(const TaskStorage & ref)
    : CCompiler::TaskInterface(ref)
    , m_images(ref.m_images)
    , m_storage(ref.m_storage)
{}


void TaskStorage::push(CImageStorage::iid_t img, Data::CColor* color, const ImageCache &cache)
{
    m_images.push_back(img);
    m_trans.push_back(color);
    m_cache.push_back(cache);
}

void TaskStorage::clear()
{
    m_images.clear();
    m_cache.clear();
    m_trans.clear();
}

CCompiler::TaskInterface::LayerInfo TaskStorage::at(size_t pos)
    { return {m_storage.getCopy(m_images[pos]), m_trans[pos], m_cache[pos]}; }




Task::Task()
    : CCompiler::TaskInterface()
    , m_images()
{}

void Task::push(const QImage & img, Data::CColor* color, const ImageCache &cache)
{
    m_images.push_back(img);
    m_trans.push_back(color);
    m_cache.push_back(cache);
}

void Task::clear()
{
    m_images.clear();
    m_cache.clear();
    m_trans.clear();
}

CCompiler::TaskInterface::LayerInfo Task::at(size_t pos)
    { return {m_images[pos], m_trans[pos], m_cache[pos]}; }
