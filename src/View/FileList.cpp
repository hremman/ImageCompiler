#include <QMessageBox>
#include <QPainter>
#include <QFile>
#include <QVector>
#include "FileList.hpp"
#include "ui_FileList.h"


#include <QMutableListIterator>


CFileList::CFileList(QStringList &files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UiFileList),
    m_files(files)
{
    ui->setupUi(this);
    QObject::connect(ui->reject, &QToolButton::clicked, this, &CFileList::reject_clicked);
    QObject::connect(ui->apply, &QToolButton::clicked, this, &CFileList::accept_clicked);
    QObject::connect(ui->add, &QToolButton::clicked, this, &CFileList::add_clicked);

    ui->plainTextEdit->setPlainText(m_files.join("\n"));

    ui->plainTextEdit->setUndoRedoEnabled(true);
    ui->plainTextEdit->setFocus();
    auto cur = ui->plainTextEdit->textCursor();
    cur.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(cur);

}

CFileList::~CFileList()
{
    delete ui;
}


void CFileList::add_clicked(bool)
{
    QStringList additional = QFileDialog::getOpenFileNames(this, "Выбрать файл...", "", "Изображения PNG (*.png)");
    if (additional.size() )
    {
        auto cur = ui->plainTextEdit->textCursor();
        cur.movePosition(QTextCursor::End);
        ui->plainTextEdit->setTextCursor(cur);
        cur.insertText("\n");
        cur.insertText(additional.join("\n"));
    }
}

void CFileList::accept_clicked(bool)
{

    QString temp = ui->plainTextEdit->toPlainText().trimmed();
    QStringList temp_list;
    if ( temp.size() )
    {
        temp_list = temp.split("\n");
        QMutableListIterator<QString> it(temp_list);
        while (it.hasNext()) {
            it.next();
            it.value() = it.value().trimmed();
            if (it.value().length() == 0)
                it.remove();
        }
    }
    QStringList errored;
    QStringList errors;
    for (QString &item: temp_list)
    {
        QFile test;
        test.setFileName(item);
        if ( !test.open(QIODeviceBase::ReadOnly) )
        {
            errored.append(item);
            errors.append(test.errorString());
        }
        else
            test.close();
    }

    if ( errored.size() )
    {
        QString text("Файлы не могут быть открыты на чтение:");
        auto errored_ = errored.begin();
        auto errors_ = errors.begin();
        for (; errored_ != errored.end() && errors_ != errors.end() ; ++errored_, ++errors_ )
            text += "\n\t" + *errored_ + " [" + *errors_ + "]";
        QMessageBox::warning(this, "Нет доступа к файлам", text);
        ui->plainTextEdit->setPlainText(temp_list.join("\n"));
        return;
    }

    m_files.swap(temp_list);
    this->accept();

}

void CFileList::reject_clicked(bool)
    {this->reject();}


