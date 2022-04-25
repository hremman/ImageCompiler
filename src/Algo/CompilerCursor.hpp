#ifndef CURSOR_HPP
#define CURSOR_HPP

#include "Compiler.hpp"

class CCompiler::Cursor
{
public:
    Cursor();

protected:
    std::vector<std::vector<CImageStorage::iid_t>> m_files;
    std::vector<std::list<Data::CColor*>& > m_colors;



};

#endif // CURSOR_HPP
