#ifndef COLORSETTINGSVIEW_H
#define COLORSETTINGSVIEW_H

#include <QDialog>
#include <QList>
#include "Data/Color.hpp"



namespace Ui {
class ColorSettings;
}

class ColorSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ColorSettings(QList<Data::CColor> *, int, QWidget *parent = nullptr);
    ~ColorSettings();

private:
    Ui::ColorSettings *ui;
    QList<Data::CColor> * m_colors;
};

#endif // COLORSETTINGSVIEW_H
