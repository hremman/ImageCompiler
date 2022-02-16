#ifndef CALGLAYER_HPP
#define CALGLAYER_HPP

#include "DataLayer.hpp"
#include "AlgImageTransformation.hpp"

class CAlgLayer
{
public:
    CAlgLayer();

protected: //fields
    CDataLayer * m_layer_data;
    CImageTransformation * m_transform;
};

#endif // CALGLAYER_HPP
