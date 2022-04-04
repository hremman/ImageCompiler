#include "ProjectStory.hpp"

Data::CProjectStory::CProjectStory(CProject & proj)
    : m_root(proj)
{
    m_storage.emplace_back(m_root);
    m_current = m_storage.begin();
}

const Data::CProject & Data::CProjectStory::getCurrent() const
{
    return *m_current;
}

Data::CProject & Data::CProjectStory::getRoot() const
{
    return m_root;
}

const Data::CProject & Data::CProjectStory::undo()
{
    if ( m_current != m_storage.begin() )
        return *(--m_current);
    else
        throw story_botom_reached("No more elements in story");

}

const Data::CProject & Data::CProjectStory::redo()
{
    if ( m_current != --(m_storage.end()) )
        return *(++m_current);
    else
        throw story_top_reached("No more elements in story");
}

void Data::CProjectStory::clear()
{
    m_storage.clear();
    m_storage.emplace_back(m_root);
    m_current = m_storage.begin();
}

bool Data::CProjectStory::haveUndo() const
    {return m_current != m_storage.begin();}
bool Data::CProjectStory::haveRedo() const
    {return m_current != --(m_storage.end());}

Data::CProject & Data::CProjectStory::commit(const CProject & ver)
{
    if (haveRedo())
    {
        auto temp_it = m_current;
        m_storage.erase(++temp_it, m_storage.end());
    }
    m_storage.push_back(ver);
    ++m_current;
    if ( m_storage.size() > __M_MAX)
        m_storage.pop_front();
    return *m_current;
}
