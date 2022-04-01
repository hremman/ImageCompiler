#ifndef QLINEEDITDC_H
#define QLINEEDITDC_H

#include <QLineEdit>

class QLineEditDC : public QLineEdit
{
public:
    explicit QLineEditDC(QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
};

#endif // QLINEEDITDC_H
