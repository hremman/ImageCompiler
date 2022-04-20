#ifndef LAYER_H
#define LAYER_H

#include <QWidget>
#include "Data/Layer.hpp"


namespace Ui {
class UiLayer;
}

class CLayer : public QWidget
{
    Q_OBJECT

public:
    explicit CLayer(Data::CLayer * const, QWidget *parent = nullptr);
    ~CLayer();
    Data::CLayer * getData()
        {return m_data;}

public:

signals:
    void changed();

private slots:
    void file_edit(bool);
    void settings_button(bool);
    void mode_changed(int);
    void name_changed();
    void noise_changed(double);
    void probe_changed(double);
    void slot_blink(bool);

private:
    Ui::UiLayer *ui;
    Data::CLayer * const m_data;
    void switchElements();
};

#endif // LAYER_H
