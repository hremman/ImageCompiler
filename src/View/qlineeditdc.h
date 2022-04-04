#ifndef QLINEEDITDC_H
#define QLINEEDITDC_H

#include <QLineEdit>

class QLineEditDC : public QLineEdit
{
    Q_OBJECT
public:
    explicit QLineEditDC(QWidget *parent = nullptr);

public:
signals:
    void mouseDoubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
};

#endif // QLINEEDITDC_H
