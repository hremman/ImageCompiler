#include "Layer.h"
#include "ui_Layer.h"
#include <QFileDialog>
#include "ColorSettings.h"
#include "FileList.hpp"


CLayer::CLayer(Data::CLayer * const data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiLayer),
    m_data(data)


{
    ui->setupUi(this);

    ui->mode_choose->setCurrentIndex(static_cast<int>(m_data->m_type));
    switchElements();

    ui->path_edit->setText(m_data->m_files.join("?"));
    ui->probe_val->setValue(m_data->m_use_probability);
    ui->noise_probe->setValue(m_data->m_noise_probability);



    QObject::connect(ui->settings, &QToolButton::clicked, this, &CLayer::settings_button);
    QObject::connect(ui->mode_choose, &QComboBox::activated, this, &CLayer::mode_changed);


}

CLayer::~CLayer()
{

    delete ui;
}

void CLayer::mode_changed(int mode)
{
    m_data->m_type = static_cast<Data::CLayer::WorkType>(mode);
    switchElements();

}

void CLayer::file_dc(bool)
{  
    CFileList window(m_data->m_files, this);
    window.resize(ui->path_edit->width(), 159);

    window.move(QWidget::mapToGlobal( ui->path_edit->pos()) + (ui->path_edit->rect().bottomLeft() + QPoint(0,1 + ui->path_edit->height() / 2)));

    if (window.exec() == QDialog::Accepted)
        ui->path_edit->setText(m_data->m_files.join("?"));
}

void CLayer::settings_button(bool)
{
    ColorSettings dial(&(m_data->m_colors),this);
    if ( dial.exec() == QDialog::Accepted )
    ;
}

void CLayer::switchElements()
{
    switch(m_data->m_type)
    {
    case Data::CLayer::WorkType::NOTHING:
        ui->settings->setHidden(true);

        ui->path_edit->setEnabled(true);
        ui->path_label->setEnabled(true);

        ui->probe_label->setEnabled(true);
        ui->probe_val->setEnabled(true);
        ui->noise_label->setEnabled(true);
        ui->noise_probe->setEnabled(true);
        break;
    case Data::CLayer::WorkType::GENERATION:
    case Data::CLayer::WorkType::ENUMERATION:
        ui->settings->setHidden(false);
        ui->settings->setEnabled(true);

        ui->path_edit->setEnabled(true);
        ui->path_label->setEnabled(true);

        ui->probe_label->setEnabled(true);
        ui->probe_val->setEnabled(true);
        ui->noise_label->setEnabled(true);
        ui->noise_probe->setEnabled(true);
        break;
    case Data::CLayer::WorkType::NO_TYPE:
    default:
        ui->settings->setHidden(true);

        ui->path_edit->setEnabled(false);
        ui->path_label->setEnabled(false);

        ui->probe_label->setEnabled(false);
        ui->probe_val->setEnabled(false);
        ui->noise_label->setEnabled(false);
        ui->noise_probe->setEnabled(false);
    }
}

