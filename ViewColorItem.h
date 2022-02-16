#ifndef VIEWCOLORITEM_H
#define VIEWCOLORITEM_H

#include <QWidget>
#include "DataColor.hpp"


namespace Ui {
class ColorListItem;
}

class ColorItemView : public QWidget
{
    Q_OBJECT

public:
    explicit ColorItemView(CDataColor *const data, QWidget *parent = nullptr);
    ~ColorItemView();

private slots:
    void button_clicked(bool);
    void change_v(int);
    void change_s(int);

private:
    Ui::ColorListItem *ui;
    CDataColor *const m_data;

    void update_button_color();
};

#endif // VIEWCOLORITEM_H
