#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

QT_BEGIN_NAMESPACE
namespace Ui { class UiMainWin; }
QT_END_NAMESPACE

class CompilerMainWin : public QMainWindow
{
    Q_OBJECT

public:
    CompilerMainWin(QWidget *parent = nullptr);
    ~CompilerMainWin();
    void resizeEvent(QResizeEvent* event);

private:
    Ui::UiMainWin *ui;

private slots:
    void button_clicked(bool);

private:
     QGraphicsScene m_scene;
     QGraphicsPixmapItem m_item;

};
#endif // MAINWINDOW_H
