#ifndef COLORSETTINGSVIEW_H
#define COLORSETTINGSVIEW_H

#include <QDialog>
#include "Data/ColorSettings.h"



namespace Ui {
class UiColorSettings;
}

class ColorSettings : public QDialog
{
    Q_OBJECT

public:
    explicit ColorSettings(Data::CColorSettings *, QWidget *parent = nullptr);
    ~ColorSettings();

private:
    Ui::UiColorSettings *ui;
    Data::CColorSettings * m_data;
    std::list<Data::CColor> m_colors;
    std::list<Data::CColor> m_new_colors;
    std::list<Data::CColor const *> m_rem_colors;

    void copyOnWrite();
private slots:
    void react_toogled_rand(bool);
    void react_toogled_enum(bool);
    void react_rejected();
    void react_accepted();
    void react_add(bool);
    void react_rem(bool);
};

#endif // COLORSETTINGSVIEW_H
