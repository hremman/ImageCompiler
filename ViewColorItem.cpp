#include "ViewColorItem.h"
#include "ui_ColorItemView.h"
#include <QColorDialog>

ColorItemView::ColorItemView(ColorItemData *const data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorListItem),
    m_data(data)
{
    ui->setupUi(this);
    ui->Value->setChecked(m_data->m_value);
    ui->Saturation->setChecked(m_data->m_saturation);

    update_button_color();

    QObject::connect(ui->Value, &QCheckBox::stateChanged, this, &ColorItemView::change_v);
    QObject::connect(ui->Saturation, &QCheckBox::stateChanged, this, &ColorItemView::change_s);
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &ColorItemView::button_clicked);

}

ColorItemView::~ColorItemView()
{
    delete ui;
}

void ColorItemView::button_clicked(bool)
{
    QColorDialog dial(m_data->m_color, this->parentWidget() );
    m_data->m_color = QColorDialog::getColor(m_data->m_color, this->parentWidget(), "Выбор цвета...", QColorDialog::ColorDialogOption::DontUseNativeDialog);

}


void ColorItemView::change_v(int state)
{
    m_data->m_value = state == Qt::Checked;
}

void ColorItemView::change_s(int state)
{
    m_data->m_value = state == Qt::Checked;
}

void ColorItemView::update_button_color()
{
    QString ss = "border: 1px solid black; background-color: rgb(" + QString::number(m_data->m_color.red()) + ", "
                                                                   + QString::number(m_data->m_color.green()) + ", "
                                                                   + QString::number(m_data->m_color.blue()) + ");";
    ui->pushButton->setStyleSheet(ss);
}



