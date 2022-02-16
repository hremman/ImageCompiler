#ifndef COLORSETTINGSVIEW_H
#define COLORSETTINGSVIEW_H

#include <QDialog>
#include "FileFormat.hpp"



namespace Ui {
class ViewColorSettings;
}

class ViewColorSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ViewColorSettings(QWidget *parent = nullptr);
    ~ViewColorSettings();

private:
    Ui::ViewColorSettings *ui;
};

#endif // COLORSETTINGSVIEW_H
