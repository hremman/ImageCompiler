#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>

namespace Ui {
class UiProjTab;
}

class ProjTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjTab(QWidget *parent = nullptr);
    ~ProjTab();

private:
    Ui::UiProjTab *ui;
};

#endif // PROJTAB_H
