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
    ui->blinked->setCheckState(m_data->m_blink ?  Qt::Checked : Qt::Unchecked);
    if(ui->blinked->isChecked())
        ui->probe_val->setEnabled(false);
    else
        ui->probe_val->setEnabled(false);


    QObject::connect(ui->settings, &QToolButton::clicked, this, &CLayer::settings_button);
    QObject::connect(ui->mode_choose, &QComboBox::activated, this, &CLayer::mode_changed);
    QObject::connect(ui->layer_name, &QLineEdit::editingFinished, this, &CLayer::name_changed);
    QObject::connect(ui->probe_val, &QDoubleSpinBox::valueChanged, this, &CLayer::probe_changed);
    QObject::connect(ui->noise_probe, &QDoubleSpinBox::valueChanged, this, &CLayer::noise_changed);
    QObject::connect(ui->path_buton, &QToolButton::clicked, this, &CLayer::file_edit);
    QObject::connect(ui->blinked, &QCheckBox::clicked, this, &CLayer::slot_blink);

    if (m_data->m_files.size())
        m_file = 0;
    else
        m_file = -1;
    if (m_data->m_colors.m_colors.size())
        m_color = 0;
    else
        m_color = -1;

}

CLayer::~CLayer()
{

    delete ui;
}


bool CLayer::preview_have_file_next() const
    { return m_file < m_data->m_files.size() - 1 && m_file != -1; }

bool CLayer::preview_have_file_prev() const
    { return m_file > 0; }

bool CLayer::preview_have_color_next() const
    { return m_color < m_data->m_colors.m_colors.size() - 1 && m_color != -1; }

bool CLayer::preview_have_color_prev() const
    { return m_color > 0; }

bool CLayer::preview_file_next()
{
    if(m_file >= m_data->m_files.size())
        m_file = m_data->m_files.size() - 1;
    if(m_file == -1 || m_file == m_data->m_files.size() - 1)
        return false;
    ++m_file;
    return true;
}

bool CLayer::preview_file_prev()
{
    if(m_file >= m_data->m_files.size())
        m_file = m_data->m_files.size() - 1;
    if(m_file <= 0)
        return false;
    --m_file;
    return true;
}

bool CLayer::preview_color_next()
{
    if(m_color >= m_data->m_colors.m_colors.size())
        m_color = m_data->m_colors.m_colors.size() - 1;
    if(m_color == -1 || m_file == m_data->m_colors.m_colors.size() - 1)
        return false;
    ++m_color;
    return true;
}

bool CLayer::preview_color_prev()
{
    if(m_color >= m_data->m_colors.m_colors.size())
        m_color = m_data->m_colors.m_colors.size() - 1;
    if(m_color <= 0)
        return false;
    --m_color;
    return true;
}

const QString * CLayer::preview_get_file() const
{

    if ( m_file == -1 )
        return nullptr;
    auto file = m_data->m_files.begin();
    for(ssize_t it = 0; it < m_file; ++it ) ++file;
    return &(*file);
}

Data::CColor * CLayer::preview_get_color() const
{
    if ( m_color == -1 )
        return nullptr;
    auto color = m_data->m_colors.m_colors.begin();
    for(ssize_t it = 0; it < m_color; ++it ) ++color;
    return &(*color);
}


void CLayer::mode_changed(int mode)
{
    m_data->m_type = static_cast<Data::CLayer::WorkType>(mode);
    if ( m_data->m_type == Data::CLayer::WorkType::NOTHING )
    {
        m_data->m_colors.m_colors.clear();
        m_color = -1;
    }
    switchElements();
    emit changed();

}

void CLayer::file_edit(bool)
{  
    QStringList temp_list = m_data->m_files;
    CFileList window(temp_list, this);
    window.resize(ui->path_edit->width(), 159);

    if (window.exec() == QDialog::Accepted)
    {
        m_data->m_files = temp_list;
        ui->path_edit->setText(m_data->m_files.join("?"));
        ui->path_edit->setToolTip(m_data->m_files.join("\n"));
        if (m_file >= m_data->m_files.size())
            m_file = m_data->m_files.size() - 1;
        if (m_file == -1 && m_data->m_files.size() != 0)
            m_file = 0;
        emit changed();
    }
}

void CLayer::settings_button(bool)
{
    ColorSettings dial(&(m_data->m_colors),this);
    if ( dial.exec() == QDialog::Accepted )
    {
        if (m_color >= m_data->m_colors.m_colors.size())
            m_color = m_data->m_colors.m_colors.size() - 1;
        if ( m_color == -1 && m_data->m_colors.m_colors.size() != 0 )
            m_color = 0;
        emit changed();
    }
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

void CLayer::slot_blink(bool)
{
    if(ui->blinked->isChecked())
        ui->probe_val->setEnabled(false);
    else
        ui->probe_val->setEnabled(true);
    if (m_data->m_blink != ui->blinked->isChecked())
    {
        m_data->m_blink = ui->blinked->isChecked();
        emit changed();
    }
}


