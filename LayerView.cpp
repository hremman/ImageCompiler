#include "LayerView.h"
#include "ui_LayerView.h"
#include <QFileDialog>
#include "ViewColorSettings.h"


CLayerView::CLayerView(LayerData * const data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CLayerView),
    m_data_inner(*data),
    m_color_inner(),
    m_data(data)

{
    ui->setupUi(this);
    ui->mode_choose->setCurrentIndex(static_cast<int>(m_data->m_mode));
    if ( m_data->m_mode == LayerMode::NO_VAL )
    {
        ui->path_choose->setEnabled(false);
        ui->path_edit->setEnabled(false);
    }

    if (m_data->m_mode == LayerMode::COLOR_ENUM)
    {
        ui->settings->setHidden(false);
        m_color_inner = *(m_data_inner.m_color);
        m_data_inner.m_color = nullptr;
    }
    else
        ui->settings->setHidden(true);
    ui->path_edit->setText(m_data->m_path);

    QObject::connect(ui->path_choose, &QToolButton::clicked, this, &CLayerView::file_button);
    QObject::connect(ui->settings, &QToolButton::clicked, this, &CLayerView::settings_button);
    QObject::connect(ui->mode_choose, &QComboBox::activated, this, &CLayerView::mode_changed);

}

CLayerView::~CLayerView()
{
    *m_data = std::move(m_data_inner);
    if ( m_data->m_mode == LayerMode::COLOR_ENUM)
        *(m_data->m_color) = m_color_inner;
    m_color_inner.m_colors = nullptr;
    delete ui;
}

void CLayerView::mode_changed(int mode)
{
    m_data_inner.m_mode = static_cast<LayerMode>(mode);
    if (m_data_inner.m_mode == LayerMode::COLOR_ENUM)
    {
        ui->settings->setHidden(false);
    }
    else
    {
        ui->settings->setHidden(true);
    }
}

void CLayerView::file_button(bool)
{
    if (m_data_inner.m_mode == LayerMode::PICTURE_ENUM)
        m_data_inner.m_path = QFileDialog::getExistingDirectory(this, "Выбор папки...", "");
    else
        m_data_inner.m_path = QFileDialog::getOpenFileName(this, "Выбрать файл...", "", "Изображения PNG (*.png)");
    ui->path_edit->setText(m_data_inner.m_path);
}

void CLayerView::settings_button(bool)
{
    ColorSettingsView dial(this);
    dial.show();
}
