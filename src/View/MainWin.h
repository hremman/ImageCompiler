#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <list>
#include "View/ProjTab.h"
#include "Data/Project.hpp"

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

    void save(const Data::CProject &) const;
    void load(Data::CProject &);
    void show_counters(ProjTab *temp = nullptr);

private:
    Ui::UiMainWin *ui;

private slots:
    void slot_create(bool);
    void slot_open(bool);
    void slot_save(bool);
    void slot_save_all(bool);
    void slot_save_copy(bool);
    void slot_close(bool);
    void slot_close_tab(int);
    void slot_close_all(bool);
    void slot_exit(bool);
    void slot_tab_edited(unsigned int);
    void slot_tab_renamed(unsigned int);
    void slot_tab_changed(int);
    void slot_redo(bool);
    void slot_undo(bool);
    void closeEvent (QCloseEvent *event);

    void slot_generate(bool);

private:
     QGraphicsScene m_scene;
     QGraphicsPixmapItem m_item;
     std::list<Data::CProject> m_projects;
     QMap<unsigned int, std::list<Data::CProject>::iterator> m_proj_index;
     QMap<unsigned int, ProjTab*> m_tabs_index;
     QMap<unsigned int, bool> m_tabs_edited;
};
#endif // MAINWINDOW_H
