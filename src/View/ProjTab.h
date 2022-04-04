#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>
#include <QModelIndexList>
#include "Data/ProjectStory.hpp"
#include "Data/Project.hpp"


namespace Ui {
class UiProjTab;
}

class ProjTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjTab(Data::CProject&, QWidget *parent = nullptr);
    ~ProjTab();

    void reload();

public slots:
    void changes();
    void name_edited();
    void path_edited();
    void folder_clicked(bool);
    void rem_clicked(bool);
    void add_clicked(bool);
    void up_clicked(bool);
    void down_clicked(bool);

    void do_redo(bool);
    void do_undo(bool);

    const Data::CProject& getLast() const;


private:
    Ui::UiProjTab *ui;
    Data::CProjectStory m_proj_v;
    Data::CProject m_proj;

};

#endif // PROJTAB_H
