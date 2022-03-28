#include "ColorSettings.h"
#include "ui_ColorSettings.h"

ColorSettings::ColorSettings(QList<Data::CColor> * colors, int num, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ColorSettings),
    m_colors(colors)
{
    ui->setupUi(this);
}

ColorSettings::~ColorSettings()
{
    delete ui;
}
