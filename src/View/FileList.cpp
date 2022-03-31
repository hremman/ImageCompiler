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
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint );
    //setWindowModality(Qt::ApplicationModal);
    QObject::connect(ui->close, &QToolButton::clicked, this, &CFileList::close_clicked);
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

void CFileList::close_clicked(bool)
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
    if ( temp_list != m_files)
    {
        m_files.swap(temp_list);
        this->accept();
    }
    else
        this->reject();
}


