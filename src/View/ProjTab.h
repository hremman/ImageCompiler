#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>

namespace Ui {
class ProjTab;
}

class ProjTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjTab(QWidget *parent = nullptr);
    ~ProjTab();

private:
    Ui::ProjTab *ui;
};

#endif // PROJTAB_H
