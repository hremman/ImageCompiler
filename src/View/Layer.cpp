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
    ui->lid->setText(QString::number(m_data->get_lid()));



    QObject::connect(ui->settings, &QToolButton::clicked, this, &CLayer::settings_button);
    QObject::connect(ui->mode_choose, &QComboBox::activated, this, &CLayer::mode_changed);
    QObject::connect(ui->layer_name, &QLineEdit::editingFinished, this, &CLayer::mode_changed);
    QObject::connect(ui->probe_val, &QDoubleSpinBox::valueChanged, this, &CLayer::mode_changed);
    QObject::connect(ui->noise_probe, &QDoubleSpinBox::valueChanged, this, &CLayer::mode_changed);


}

CLayer::~CLayer()
{

    delete ui;
}

void CLayer::mode_changed(int mode)
{
    m_data->m_type = static_cast<Data::CLayer::WorkType>(mode);
    if ( m_data->m_type == Data::CLayer::WorkType::NOTHING )
    {
        m_data->m_colors.m_colors.clear();
    }
    switchElements();
    emit changed();

}

void CLayer::file_dc()
{  
    CFileList window(m_data->m_files, this);
    window.resize(ui->path_edit->width(), 159);

    window.move(QWidget::mapToGlobal( ui->path_edit->pos()) + (ui->path_edit->rect().bottomLeft() + QPoint(0,1 + ui->path_edit->height() / 2)));

    if (window.exec() == QDialog::Accepted)
    {
        ui->path_edit->setText(m_data->m_files.join("?"));
        emit changed();
    }
}

void CLayer::settings_button(bool)
{
    ColorSettings dial(&(m_data->m_colors),this);
    if ( dial.exec() == QDialog::Accepted )
        emit changed();
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

void CLayer::name_changed()
{
    if (m_data->m_name != ui->layer_name->text())
    {
        m_data->m_name = ui->layer_name->text();
        emit changed();
    }
}

void CLayer::noise_changed(double)
{
    if (m_data->m_noise_probability != ui->noise_probe->value())
    {
        m_data->m_noise_probability = ui->noise_probe->value();
        emit changed();
    }
}

void CLayer::probe_changed(double)
{
    if (m_data->m_use_probability != ui->probe_val->value())
    {
        m_data->m_use_probability = ui->probe_val->value();
        emit changed();
    }
}


