#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Compiler.hpp"

class CCompiler::Cursor
{
public:
    using Sequence = std::vector<std::pair<CImageStorage::iid_t, Data::CColor*>>;
    Cursor();
    bool get_next(Sequence &);



    std::vector<size_t> m_current_file;
    std::vector<std::list<Data::CColor>::iterator> m_current_color;

    std::vector<std::vector<CImageStorage::iid_t>> m_files;
    std::vector<std::list<Data::CColor> *> m_colors;

private:
    bool m_first;
};

#endif // CURSOR_HPP
