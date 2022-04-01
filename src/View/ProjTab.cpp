#include "ProjTab.h"
#include "ui_ProjTab.h"

#include <QPushButton>

ProjTab::ProjTab(Data::CProject& proj,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiProjTab),
    m_proj(proj),
    m_story(m_proj)
{
    ui->setupUi(this);
    QObject::connect(ui->proj_name, &QLineEdit::editingFinished, this, &ProjTab::name_edited);
    QObject::connect(ui->out_path, &QLineEdit::editingFinished, this, &ProjTab::path_edited);
    QObject::connect(ui->path_choose, &QToolButton::clicked, this, &ProjTab::folder_clicked);
    QObject::connect(ui->rem, &QToolButton::clicked, this, &ProjTab::rem_clicked);
    QObject::connect(ui->add, &QToolButton::clicked, this, &ProjTab::add_clicked);

/*
    // Add to list a new item (item is simply an entry in your list)
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);

    for ( int i = 0; i < ui->listWidget->count(); i ++)
    {
        ui->listWidget->item(i)->setSizeHint(QSize(100,50));
    }

    ui->listWidget->addItem(item);

    // Instanciate a custom widget
    QPushButton *row = new QPushButton("Кнопка");
    row->setMinimumSize(100,50);

    //item->setSizeHint(row->size());
    //item->setSizeHint(row->minimumSizeHint());


    // Associate the custom widget to the list entry
    ui->listWidget->setItemWidget(item, row);
*/
}

ProjTab::~ProjTab()
{
    delete ui;
}
