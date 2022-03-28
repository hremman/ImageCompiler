#ifndef LAYER_H
#define LAYER_H

#include <QWidget>
#include "Data/Layer.hpp"

namespace Ui {
class CLayer;
}

class CLayer : public QWidget
{
    Q_OBJECT

public:
    explicit CLayer(Data::CLayer * const, QWidget *parent = nullptr);
    ~CLayer();
    bool isChanged() const;
private slots:
    void file_dc(bool);
    void settings_button(bool);
    void mode_changed(int);

private:
    Ui::CLayer *ui;
    Data::CLayer * const m_data;
    bool m_changed;

    void switchElements();
};

#endif // LAYER_H
