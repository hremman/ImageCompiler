#ifndef FILELIST_H
#define FILELIST_H

#include <QDialog>
#include <QStringList>
#include <QFileDialog>


namespace Ui {
class UiFileList;
}

class CFileList : public QDialog
{
    Q_OBJECT

public:
    explicit CFileList(QStringList &, QWidget *parent = nullptr);
    ~CFileList();

private slots:
    void add_clicked(bool);
    void accept_clicked(bool);
    void reject_clicked(bool);

private:
    Ui::UiFileList *ui;
    QStringList &m_files;


};



#endif // FILELIST_H
