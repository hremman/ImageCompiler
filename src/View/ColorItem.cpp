#include "ColorItem.h"
#include "ui_ColorItem.h"
#include <QColorDialog>

ColorItem::ColorItem(Data::CColor *const data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiColorItem),
    m_data(data)
{
    ui->setupUi(this);
    ui->Value->setChecked(m_data->m_value);
    ui->Saturation->setChecked(m_data->m_saturation);

    update_button_color();

    QObject::connect(ui->Value, &QCheckBox::stateChanged, this, &ColorItem::change_v);
    QObject::connect(ui->Saturation, &QCheckBox::stateChanged, this, &ColorItem::change_s);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &ColorItem::button_clicked);

}

ColorItem::~ColorItem()
{
    delete ui;
}

void ColorItem::button_clicked(bool)
{
    QColorDialog dial(m_data->m_color, this->parentWidget() );
    m_data->m_color = QColorDialog::getColor(m_data->m_color, this->parentWidget(), "Выбор цвета...", QColorDialog::ColorDialogOption::DontUseNativeDialog);
    update_button_color();

}


void ColorItem::change_v(int state)
{
    m_data->m_value = state == Qt::Checked;
}

void ColorItem::change_s(int state)
{
    m_data->m_value = state == Qt::Checked;
}

void ColorItem::update_button_color()
{
    QString ss = "border: 1px solid black; background-color: rgb(" + QString::number(m_data->m_color.red()) + ", "
                                                                   + QString::number(m_data->m_color.green()) + ", "
                                                                   + QString::number(m_data->m_color.blue()) + ");";
    ui->pushButton->setStyleSheet(ss);
}



