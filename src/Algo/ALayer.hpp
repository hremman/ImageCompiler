#ifndef CALGLAYER_HPP
#define CALGLAYER_HPP

#include "Data/DLayer.hpp"
#include "ImageTransformation.hpp"

class CAlgLayer
{
public:
    CAlgLayer();

protected: //fields
    Data::CLayer * m_layer_data;
    CImageTransformation * m_transform;
};

#endif // CALGLAYER_HPP
