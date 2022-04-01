#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>
#include <Data/ProjectStory.hpp>
#include "Data/Project.hpp"


namespace Ui {
class UiProjTab;
}

class ProjTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjTab(QWidget *parent = nullptr);
    ~ProjTab();

    void rebuild();

private:
    Ui::UiProjTab *ui;
    Data::CProjectStory m_story;

};

#endif // PROJTAB_H
