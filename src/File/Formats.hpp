#ifndef FORMATS_H
#define FORMATS_H

#include <QColor>
#include <QString>
#include <QList>

namespace File {

enum class LayerMode
{
    NO_VAL = -1,
    NOCHANGE = 0,
    PICTURE_ENUM,
    COLOR_ENUM,
};


#pragma pack(1)
struct color_item_file
{
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
    unsigned char m_saturation:1;
    unsigned char m_value:1;
};

struct layer_data_file
{
    unsigned char m_mode;
    unsigned short m_name_len;
    unsigned int m_name_pos;
    unsigned short m_path_len;
    unsigned int m_path_pos;
    unsigned int m_color;
};


struct color_settings_file
{
    unsigned int m_mode:1;
    unsigned int m_gen_noise:1;
    unsigned int m_gen_noise_p:7;
    unsigned int m_mix_noise:1;
    unsigned int m_mix_noise_p:7;
    unsigned int m_color_num:14;
    unsigned int m_color_pos;
};

}

#pragma pack()
#endif // FORMATS_H
