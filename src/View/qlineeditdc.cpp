#include "qlineeditdc.h"

QLineEditDC::QLineEditDC(QWidget *parent)
    : QLineEdit(parent)
{}


void QLineEditDC::mouseDoubleClickEvent(QMouseEvent *e)
{
    (void)e;
    emit mouseDoubleClicked();
}
