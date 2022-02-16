#include "ViewColorSettings.h"
#include "ui_ColorSettingsView.h"

ViewColorSettings::ViewColorSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewColorSettings)
{
    ui->setupUi(this);
}

ViewColorSettings::~ViewColorSettings()
{
    delete ui;
}
