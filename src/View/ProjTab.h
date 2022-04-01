#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>
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


private:
    Ui::UiProjTab *ui;
    Data::CProject &m_proj;
    Data::CProjectStory m_story;

};

#endif // PROJTAB_H
