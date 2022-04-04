#include <QPushButton>
#include <QFileDialog>
#include <algorithm>

#include "ProjTab.h"
#include "ui_ProjTab.h"
#include "View/Layer.h"


ProjTab::ProjTab(Data::CProject& proj,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiProjTab),
    m_proj_v(proj),
    m_proj(m_proj_v.getCurrent())
{
    ui->setupUi(this);
    QObject::connect(ui->proj_name, &QLineEdit::editingFinished, this, &ProjTab::name_edited);
    QObject::connect(ui->out_path, &QLineEdit::editingFinished, this, &ProjTab::path_edited);
    QObject::connect(ui->path_choose, &QToolButton::clicked, this, &ProjTab::folder_clicked);
    QObject::connect(ui->rem, &QToolButton::clicked, this, &ProjTab::rem_clicked);
    QObject::connect(ui->add, &QToolButton::clicked, this, &ProjTab::add_clicked);
    QObject::connect(ui->up, &QToolButton::clicked, this, &ProjTab::up_clicked);
    QObject::connect(ui->down, &QToolButton::clicked, this, &ProjTab::down_clicked);
    ui->layer_list->setDragEnabled(false);

}

ProjTab::~ProjTab()
{
    delete ui;
}


void ProjTab::changes()
{
    m_proj = m_proj_v.commit(m_proj);
}

void ProjTab::name_edited()
{
    m_proj.m_name = ui->proj_name->text();
    changes();
}

void ProjTab::path_edited()
{
    m_proj.m_out_path = ui->out_path->text();
    changes();
}

void ProjTab::folder_clicked(bool)
{
    QString temp = QFileDialog::getExistingDirectory(this, "Директория вывода...", m_proj.m_out_path);
    if (temp != "" && m_proj.m_out_path != temp )
    {
        m_proj.m_out_path = temp;
        ui->out_path->setText(temp);
        changes();
    }

}

void ProjTab::rem_clicked(bool)
{
    QList<QListWidgetItem *> selected = ui->layer_list->selectedItems();
    bool do_changes = selected.count() != 0;
    for (auto i = selected.begin(); i != selected.end(); i++)
    {
        CLayer* temp = static_cast<CLayer*>(ui->layer_list->itemWidget(*i));
        m_proj.removeLayer(temp->getData());
        temp->disconnect();
        delete ui->layer_list->itemWidget(*i);
        ui->layer_list->removeItemWidget(*i);
        auto j = ui->layer_list->takeItem(ui->layer_list->row(*i));
        delete j;
    }
    if (do_changes)
        changes();
}

void ProjTab::add_clicked(bool)
{
    QListWidgetItem *item = new QListWidgetItem(ui->layer_list);
    ui->layer_list->addItem(item);
    CLayer * new_item = new CLayer(m_proj.newLayer());
    item->setSizeHint(new_item->minimumSizeHint());
    ui->layer_list->setItemWidget(item, new_item);
    QObject::connect(new_item, &CLayer::changed, this, &ProjTab::changes);
    changes();
}

void ProjTab::do_redo(bool)
{
    if (m_proj_v.haveRedo())
        m_proj = m_proj_v.redo();
    reload();
}

void ProjTab::do_undo(bool)
{
    if (m_proj_v.haveUndo())
        m_proj = m_proj_v.undo();
    reload();
}

const Data::CProject& ProjTab::getLast() const
    {return m_proj_v.getCurrent();}

void ProjTab::up_clicked(bool)
{
    QList<QListWidgetItem*> selected = ui->layer_list->selectedItems();
    std::sort(selected.begin(), selected.end(), [this](const QListWidgetItem* a, const QListWidgetItem* b) {
        return ui->layer_list->row(a) < ui->layer_list->row(b);
    });
    int prev = -1;
    QVector<int> selected_rows;
    for (auto it: selected )
    {
        int row = ui->layer_list->row(it);
        if (row - 1 != prev)
        {

            QWidget * widget = new CLayer(static_cast<CLayer*>(ui->layer_list->itemWidget(it))->getData());
            QListWidgetItem* item = ui->layer_list->takeItem(row);
            ui->layer_list->removeItemWidget(item);
            ui->layer_list->insertItem(row - 1, item);
            ui->layer_list->setItemWidget(item, widget);
            m_proj.moveLayerUp(row);
            changes();
        }
        prev = ui->layer_list->row(it);
        selected_rows.append(prev);
    }
    ui->layer_list->setCurrentRow(0, QItemSelectionModel::Deselect);
    for (int it: selected_rows)
    {
        ui->layer_list->setCurrentRow(it, QItemSelectionModel::Select);
    }

}

void ProjTab::down_clicked(bool)
{
    QList<QListWidgetItem*> selected = ui->layer_list->selectedItems();
    std::sort(selected.begin(), selected.end(), [this](const QListWidgetItem* a, const QListWidgetItem* b) {
        return ui->layer_list->row(a) > ui->layer_list->row(b);
    });
    int prev = ui->layer_list->count();
    QVector<int> selected_rows;
    for (auto it: selected )
    {
        int row = ui->layer_list->row(it);
        if (row + 1 != prev)
        {

            QWidget * widget = new CLayer(static_cast<CLayer*>(ui->layer_list->itemWidget(it))->getData());
            QListWidgetItem* item = ui->layer_list->takeItem(row);
            ui->layer_list->removeItemWidget(item);
            ui->layer_list->insertItem(row + 1, item);
            ui->layer_list->setItemWidget(item, widget);
            m_proj.moveLayerDown(row);
            changes();
        }
        prev = ui->layer_list->row(it);
        selected_rows.append(prev);
    }
    ui->layer_list->setCurrentRow(0, QItemSelectionModel::Deselect);
    for (int it: selected_rows)
    {
        ui->layer_list->setCurrentRow(it, QItemSelectionModel::Select);
    }

}


void ProjTab::reload()
{

}
