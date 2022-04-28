#include "CompilerCursor.hpp"

CCompiler::Cursor::Cursor()
    : m_current_file()
    , m_current_color()
    , m_files()
    , m_colors()
    , m_first(true)
{

}


bool CCompiler::Cursor::get_next(Sequence &out)
{
    if ( m_current_file[0] == m_files[0].size() )
        return false;
    if (!m_first)
    {
        for (size_t layer = m_files.size() - 1; layer != 0; --layer )
        {
            m_current_color[layer]++;
            if(m_current_color[layer] == m_colors[layer]->end())
            {
                m_current_color[layer] = m_colors[layer]->begin();
                ++m_current_file[layer];
                if ( m_current_file[layer] == m_files[layer].size() )
                {
                    m_current_file[layer] = 0;
                    continue;
                }
                break;
            }
            break;
        }
    }
    else
        m_first = false;
    out.resize(m_current_file.size());
    for (size_t it = m_current_file.size() - 1; it != 0; ++it )
        out[it] = std::make_pair(m_files[it][m_current_file[it]], ( m_current_color[it] == m_colors[it]->end()? nullptr : &(*m_current_color[it])));
    return true;
}
