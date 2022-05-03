#ifndef CPREVIEWBUILDER_HPP
#define CPREVIEWBUILDER_HPP

#include <vector>
#include <QImage>
#include "Algo/Compiler.hpp"
#include "Data/Color.hpp"


class CPreviewBuilder
{
public:
    static QImage buildPreview(std::vector<QString>, std::vector<Data::CColor *>);
};

#endif // CPREVIEWBUILDER_HPP
