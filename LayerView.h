#ifndef LAYERVIEW_H
#define LAYERVIEW_H

#include <QWidget>

#include "FileFormat.hpp"

namespace Ui {
class CLayerView;
}

class CLayerView : public QWidget
{
    Q_OBJECT

public:
    explicit CLayerView(LayerData * const, QWidget *parent = nullptr);
    ~CLayerView();
private slots:
    void file_button(bool);
    void settings_button(bool);
    void mode_changed(int);

private:
    Ui::CLayerView *ui;
    LayerData m_data_inner;
    ColorSettingsData m_color_inner;
    LayerData * const m_data;
};

#endif // LAYERVIEW_H
