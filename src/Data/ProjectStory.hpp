#ifndef CSAVESTORY_HPP
#define CSAVESTORY_HPP
#include <list>
#include <stdexcept>
#include "Project.hpp"

namespace Data {

class CProjectStory
{
public:
    CProjectStory(CProject &);

    CProject & commit(const CProject &);
    const CProject & undo();
    const CProject & redo();
    bool haveUndo() const;
    bool haveRedo() const;
    void clear();
    const CProject & getCurrent() const;
    CProject & getRoot() const;

protected:
    std::list<Data::CProject> m_storage;
    std::list<Data::CProject>::iterator m_current;
    std::list<Data::CProject>::iterator m_last;
    size_t m_pos;
    Data::CProject &m_root;
    static const unsigned short __M_MAX = 128;
};

}
#endif // CSAVESTORY_HPP
