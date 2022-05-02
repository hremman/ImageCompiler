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
    bool preview_have_file_next() const;
    bool preview_have_file_prev() const;
    bool preview_have_color_next() const;
    bool preview_have_color_prev() const;
    bool preview_file_next();
    bool preview_file_prev();
    bool preview_color_next();
    bool preview_color_prev();
    const QString * preview_get_file() const;
    Data::CColor * preview_get_color() const;

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

    ssize_t m_file;
    ssize_t m_color;
};

#endif // LAYER_H
