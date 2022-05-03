#include "ColorSettings.h"
#include "ui_ColorSettings.h"
#include "ColorItem.h"

ColorSettings::ColorSettings(Data::CColorSettings * data, QWidget *parent):
    QDialog(parent),
    ui(new Ui::UiColorSettings),
    m_data(data),
    m_new_colors()
{
    ui->setupUi(this);

    if (m_data->m_mode == Data::CColorSettings::Mode::GENERATION)
    {
        ui->colors_list->setEnabled(false);
        ui->tb_add->setEnabled(false);
        ui->tb_rem->setEnabled(false);

        ui->generation_number->setEnabled(true);
        ui->random_saturation->setEnabled(true);
        ui->random_value->setEnabled(true);
        ui->generation_number->setValue(m_data->m_generations_number);
        ui->random_saturation->setCheckState(m_data->m_saturatiom ?  Qt::Checked : Qt::Unchecked);
        ui->random_value->setCheckState(m_data->m_value ?  Qt::Checked : Qt::Unchecked);
        ui->random_ch->setChecked(true);
        ui->enumer_ch->setChecked(false);
    }
    else
    {
        ui->generation_number->setEnabled(false);
        ui->random_saturation->setEnabled(false);
        ui->random_value->setEnabled(false);

        ui->colors_list->setEnabled(true);
        ui->tb_add->setEnabled(true);
        ui->tb_rem->setEnabled(true);
        m_colors = m_data->m_colors;
        for(auto i = m_colors.begin(); i != m_colors.end(); i++)
        {
            // Add to list a new item (item is simply an entry in your list)
            QListWidgetItem *item = new QListWidgetItem(ui->colors_list);

            ui->colors_list->addItem(item);

            // Instanciate a custom widget
            ColorItem *row = new ColorItem(&(*i), this);

            item->setSizeHint(row->minimumSizeHint());


            // Associate the custom widget to the list entry
            ui->colors_list->setItemWidget(item, row);
        }
        ui->random_ch->setChecked(false);
        ui->enumer_ch->setChecked(true);
    }
    QObject::connect(ui->random_ch, &QRadioButton::clicked, this, &ColorSettings::react_toogled_rand);
    QObject::connect(ui->enumer_ch, &QRadioButton::clicked, this, &ColorSettings::react_toogled_enum);
    QObject::connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ColorSettings::react_accepted);
    QObject::connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &ColorSettings::react_rejected);
    QObject::connect(ui->tb_add, &QToolButton::clicked, this, &ColorSettings::react_add);
    QObject::connect(ui->tb_rem, &QToolButton::clicked, this, &ColorSettings::react_rem);
}

ColorSettings::~ColorSettings()
{
    delete ui;
}
void ColorSettings::react_toogled_enum(bool)
{
    if ( ui->enumer_ch->isChecked() )
    {
        ui->colors_list->setEnabled(true);
        ui->tb_add->setEnabled(true);
        ui->tb_rem->setEnabled(true);

        ui->generation_number->setEnabled(false);
        ui->random_saturation->setEnabled(false);
        ui->random_value->setEnabled(false);
        ui->for_each->setEnabled(false);

        ui->random_ch->setChecked(false);
    }
    else
    {
        ui->colors_list->setEnabled(false);
        ui->tb_add->setEnabled(false);
        ui->tb_rem->setEnabled(false);

        ui->generation_number->setEnabled(true);
        ui->random_saturation->setEnabled(true);
        ui->random_value->setEnabled(true);
        ui->for_each->setEnabled(true);

        ui->random_ch->setChecked(true);
    }
}
void ColorSettings::react_toogled_rand(bool)
{
    if ( ui->random_ch->isChecked() )
    {


        ui->colors_list->setEnabled(false);
        ui->tb_add->setEnabled(false);
        ui->tb_rem->setEnabled(false);

        ui->generation_number->setEnabled(true);
        ui->random_saturation->setEnabled(true);
        ui->random_value->setEnabled(true);
        ui->for_each->setEnabled(true);

        ui->enumer_ch->setChecked(false);
    }
    else
    {
        ui->colors_list->setEnabled(true);
        ui->tb_add->setEnabled(true);
        ui->tb_rem->setEnabled(true);

        ui->generation_number->setEnabled(false);
        ui->random_saturation->setEnabled(false);
        ui->random_value->setEnabled(false);
        ui->for_each->setEnabled(false);

        ui->enumer_ch->setChecked(true);
    }
}

void ColorSettings::react_accepted()
{
    qDebug() << QString("random: %1").arg(ui->random_ch->isChecked());
    qDebug() << QString("enum: %1").arg(ui->enumer_ch->isChecked());
    if ( ui->random_ch->isChecked() )
    {
        m_data->m_mode = Data::CColorSettings::Mode::GENERATION;
        m_data->m_colors.clear();
        m_data->m_saturatiom = ui->random_saturation->isChecked();
        m_data->m_value = ui->random_value->isChecked();
        m_data->m_generations_number = ui->generation_number->value();
        m_data->m_for_each = ui->for_each->isChecked();
    }
    else
    {
        m_data->m_mode = Data::CColorSettings::Mode::ENUMERATION;
        for (auto i = m_rem_colors.begin(); i != m_rem_colors.end(); i++)
            m_colors.remove(**i);
        for (auto i = m_new_colors.begin(); i != m_new_colors.end(); i++)
            m_colors.emplace_back(*i);

        m_data->m_colors = std::move(m_colors);
    }
    this->accept();
}

void ColorSettings::react_rejected()
{
    this->reject();
}

void ColorSettings::react_add(bool)
{
    QListWidgetItem *item = new QListWidgetItem(ui->colors_list);

    ui->colors_list->addItem(item);

    m_new_colors.emplace_front();
    ColorItem *row = new ColorItem(&(m_new_colors.front()), this);

    item->setSizeHint(row->minimumSizeHint());

    ui->colors_list->setItemWidget(item, row);
}

void ColorSettings::react_rem(bool)
{
    QList<QListWidgetItem *> selected = ui->colors_list->selectedItems();
    for (auto i = selected.begin(); i != selected.end(); i++)
    {
        m_rem_colors.push_back(static_cast<ColorItem*>(ui->colors_list->itemWidget(*i))->getData());

        ui->colors_list->removeItemWidget(*i);
        auto j = ui->colors_list->takeItem(ui->colors_list->row(*i));
        delete j;
    }
}



