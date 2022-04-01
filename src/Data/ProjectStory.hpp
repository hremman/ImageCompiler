#ifndef CSAVESTORY_HPP
#define CSAVESTORY_HPP
#include <list>
#include "Project.hpp"

namespace Data {

class CProjectStory
{
public:
    CProjectStory(const CProject &);

    void commit(const CProject &);
    const CProject & undo();
    const CProject & redo();
    void clear();

protected:
    std::list<Data::CProject> m_storage;
    std::list<Data::CProject>::iterator m_current;
    size_t m_pos;
    static const unsigned short __M_MAX = 128;
};

}
#endif // CSAVESTORY_HPP
