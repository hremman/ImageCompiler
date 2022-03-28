#ifndef COLORITEM_H
#define COLORITEM_H

#include <QWidget>
#include "Data/Color.hpp"


namespace Ui {
class ColorItem;
}

class ColorItem : public QWidget
{
    Q_OBJECT

public:
    explicit ColorItem(Data::CColor *const data, QWidget *parent = nullptr);
    ~ColorItem();

private slots:
    void button_clicked(bool);
    void change_v(int);
    void change_s(int);

private:
    Ui::ColorItem *ui;
    Data::CColor *const m_data;

    void update_button_color();
};

#endif // COLORITEM_H
