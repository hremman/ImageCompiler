#include "ProjTab.h"
#include "ui_ProjTab.h"

#include <QPushButton>

ProjTab::ProjTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiProjTab)
{
    ui->setupUi(this);

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
