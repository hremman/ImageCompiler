#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>
#include <QModelIndexList>
#include <QSet>
#include <stdexcept>
#include "Data/ProjectStory.hpp"
#include "Data/Project.hpp"


namespace Ui {
class UiProjTab;
}

class to_big_id: std::runtime_error
{
public:
    to_big_id(const std::string & msg) : std::runtime_error(msg) {}
};

class ProjTab : public QWidget
{
    Q_OBJECT

public:
    explicit ProjTab(Data::CProject&, QWidget *parent = nullptr);
    ~ProjTab();

    void reload();
    void do_redo(bool);
    void do_undo(bool);

    bool have_undo();
    bool have_redo();


    const Data::CProject& getLast() const;
    int getId() const
        {return m_id;}

public slots:
    void changes();
    void name_edited();
    void path_edited();
    void folder_clicked(bool);
    void rem_clicked(bool);
    void add_clicked(bool);
    void up_clicked(bool);
    void down_clicked(bool);
    void slot_select_changed();

public:
signals:
    void changed(unsigned int);
    void project_renamed(unsigned int);

public:
    int m_number;
    void count_vars();
private:
    Ui::UiProjTab *ui;
    Data::CProjectStory m_proj_v;
    Data::CProject m_proj;
    unsigned int m_id;

private:
    static QSet<unsigned int> __M_used_id;
};

#endif // PROJTAB_H
