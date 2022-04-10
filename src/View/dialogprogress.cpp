#include "dialogprogress.h"
#include "ui_dialogprogress.h"

DialogProgress::DialogProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, 25);
    ui->tableWidget->setColumnWidth(1, 422);
}

DialogProgress::~DialogProgress()
{
    delete ui;
}
