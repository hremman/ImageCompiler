#ifndef COLORITEM_H
#define COLORITEM_H

#include <QWidget>
#include "Data/Color.hpp"


namespace Ui {
class UiColorItem;
}

class ColorItem : public QWidget
{
    Q_OBJECT

public:
    explicit ColorItem(Data::CColor *const data, QWidget *parent = nullptr);
    ~ColorItem();
    Data::CColor const * getData()
        {return m_data;}

private slots:
    void button_clicked(bool);
    void change_v(int);
    void change_s(int);

private:
    Ui::UiColorItem *ui;
    Data::CColor *const m_data;

    void update_button_color();
};

#endif // COLORITEM_H
