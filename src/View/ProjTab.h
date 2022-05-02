#ifndef PROJTAB_H
#define PROJTAB_H

#include <QWidget>
#include <QModelIndexList>
#include <QSet>
#include <stdexcept>
#include "Data/ProjectStory.hpp"
#include "Data/Project.hpp"
#include "Algo/CompilerTask.hpp"

class CLayer;

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
    unsigned int getId() const
        {return m_id;}

    void count_vars();
    const CLayer* get_single_selected();

    void task(std::vector<QString> &, std::vector<Data::CColor *> &);
    bool next_file();
    bool prev_file();
    bool next_color();
    bool prev_color();


public slots:
    void slot_changes();
    void slot_name_edited();
    void slot_path_edited();
    void slot_folder(bool);
    void slot_rem(bool);
    void slot_add(bool);
    void slot_up(bool);
    void slot_down(bool);
    void slot_select_changed();

public:
signals:
    void sig_changed(unsigned int);
    void sig_project_renamed(unsigned int, const QString &);
    void sig_can_change_preview(unsigned int, bool);

public:
    int m_number;
    bool m_preview;
private:
    Ui::UiProjTab *ui;
    Data::CProjectStory m_proj_v;
    Data::CProject m_proj;
    unsigned int m_id;

private:
    static QSet<unsigned int> __M_used_id;
};

#endif // PROJTAB_H
