#ifndef CALGLAYER_HPP
#define CALGLAYER_HPP

#include "Data/Layer.hpp"
#include "old_ImageTransformation.hpp"

class CLayer
{
public:
    CLayer();

protected: //fields
    Data::CLayer * m_layer_data;
    CImageTransform * m_transform;
};

#endif // CALGLAYER_HPP
