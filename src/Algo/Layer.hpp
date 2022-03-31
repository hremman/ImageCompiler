#ifndef CALGLAYER_HPP
#define CALGLAYER_HPP

#include "Data/Layer.hpp"
#include "ImageTransformation.hpp"

class CLayer
{
public:
    CLayer();

protected: //fields
    Data::CLayer * m_layer_data;
    CImageTransform * m_transform;
};

#endif // CALGLAYER_HPP
