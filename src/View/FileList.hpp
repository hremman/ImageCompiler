#ifndef FILELIST_H
#define FILELIST_H

#include <QDialog>
#include <QDialog>
#include <QStringList>
#include <QFileDialog>


namespace Ui {
class CFileList;
}

class CFileList : public QDialog
{
    Q_OBJECT

public:
    explicit CFileList(QStringList &, QWidget *parent = nullptr);
    ~CFileList();

private slots:
    void add_clicked(bool);
    void close_clicked(bool);

private:
    Ui::CFileList *ui;
    QStringList &m_files;
};



#endif // FILELIST_H
