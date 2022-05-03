#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

#include <algorithm>

#include "Exceptions.hpp"
#include "ProjTab.h"
#include "ui_ProjTab.h"
#include "View/Layer.h"

QSet<unsigned int> ProjTab::__M_used_id;

ProjTab::ProjTab(Data::CProject& proj, QWidget *parent) :
    QWidget(parent),
    m_number(0),
    m_preview(false),
    m_preview_img(),
    ui(new Ui::UiProjTab),
    m_proj_v(proj),
    m_proj(m_proj_v.getCurrent()),
    m_id()

{
    for (unsigned int i = 0; i <= UINT_MAX - 2 ; i++)
    {
        if ( !(__M_used_id.contains(i)) )
        {
            __M_used_id.insert(i);
            m_id = i;
            break;
        }
        if ( i == UINT_MAX - 2 )
        {
            m_id = i;   //Для корректного удаления объекта.
            throw too_many_id("Превышен лимит идентификаторов для объекта ProjTab");
        }
    }
    ui->setupUi(this);
    QObject::connect(ui->proj_name, &QLineEdit::editingFinished, this, &ProjTab::slot_name_edited);
    QObject::connect(ui->out_path, &QLineEdit::editingFinished, this, &ProjTab::slot_path_edited);
    QObject::connect(ui->path_choose, &QToolButton::clicked, this, &ProjTab::slot_folder);
    QObject::connect(ui->rem, &QToolButton::clicked, this, &ProjTab::slot_rem);
    QObject::connect(ui->add, &QToolButton::clicked, this, &ProjTab::slot_add);
    QObject::connect(ui->up, &QToolButton::clicked, this, &ProjTab::slot_up);
    QObject::connect(ui->down, &QToolButton::clicked, this, &ProjTab::slot_down);
    QObject::connect(ui->layer_list, &QListWidget::itemSelectionChanged, this, &ProjTab::slot_select_changed);

    ui->layer_list->setDragEnabled(false);
    ui->down->setEnabled(false);
    ui->up->setEnabled(false);
    reload();

}

ProjTab::~ProjTab()
{
    __M_used_id.remove(m_id);
    delete ui;
}


void ProjTab::slot_changes()
{
    m_proj = m_proj_v.commit(m_proj);
    count_vars();
    emit sig_changed(m_id);
}

void ProjTab::slot_name_edited()
{
    m_proj.m_name = ui->proj_name->text();
    emit sig_project_renamed(m_id, m_proj.m_name);
    slot_changes();
}

void ProjTab::slot_path_edited()
{
    m_proj.m_out_path = ui->out_path->text();
    slot_changes();
}

void ProjTab::slot_folder(bool)
{
    QString temp = QFileDialog::getExistingDirectory(this, "Директория вывода...", m_proj.m_out_path);
    if (temp != "" && m_proj.m_out_path != temp )
    {
        m_proj.m_out_path = temp;
        ui->out_path->setText(temp);
        slot_changes();
    }

}

void ProjTab::slot_rem(bool)
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
        slot_changes();
}

void ProjTab::slot_add(bool)
{
    CLayer * new_item = nullptr;
    try { new_item = new CLayer(m_proj.newLayer()); }
    catch (const too_many_id & e)
    {
        QMessageBox::warning(this, "Невозможно создать новй слой", "Возникла ошибка:\n" + e.message() + "\nСлой создан не будет. Не стоит превышать число слоёв в 9999.");
        return;
    }


    QListWidgetItem *item = new QListWidgetItem(ui->layer_list);
    ui->layer_list->addItem(item);

    item->setSizeHint(new_item->minimumSizeHint());
    ui->layer_list->setItemWidget(item, new_item);
    QObject::connect(new_item, &CLayer::changed, this, &ProjTab::slot_changes);
    slot_changes();
}

void ProjTab::do_redo(bool)
{
    if (m_proj_v.haveRedo())
        m_proj = m_proj_v.redo();
    reload();
    emit sig_project_renamed(m_id, m_proj.m_name);
    emit sig_changed(m_id);
}

void ProjTab::do_undo(bool)
{
    if (m_proj_v.haveUndo())
        m_proj = m_proj_v.undo();
    reload();
    emit sig_project_renamed(m_id, m_proj.m_name);
    emit sig_changed(m_id);
}

const Data::CProject& ProjTab::getLast() const
    {return m_proj_v.getCurrent();}

void ProjTab::slot_up(bool)
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
            slot_changes();
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

void ProjTab::slot_down(bool)
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
            delete ui->layer_list->itemWidget(item);
            ui->layer_list->removeItemWidget(item);
            ui->layer_list->insertItem(row + 1, item);
            ui->layer_list->setItemWidget(item, widget);
            m_proj.moveLayerDown(row);
            slot_changes();
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
    ui->out_path->setText(m_proj.m_out_path);
    ui->proj_name->setText(m_proj.m_name);
    for (int it = 0; it < ui->layer_list->count(); it++)
    {
        CLayer* temp = static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->item(it)));
        temp->disconnect();
        delete temp;
        ui->layer_list->removeItemWidget(ui->layer_list->item(it));
    }
    ui->layer_list->clear();
    const std::list<Data::CLayer *> & layers = m_proj.layers();
    for (auto it: layers)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->layer_list);
        ui->layer_list->addItem(item);
        CLayer * new_item = new CLayer(it);
        item->setSizeHint(new_item->minimumSizeHint());
        ui->layer_list->setItemWidget(item, new_item);
        QObject::connect(new_item, &CLayer::changed, this, &ProjTab::slot_changes);
    }
    slot_select_changed();
}

const CLayer*  ProjTab::get_single_selected()
{
    QList<QListWidgetItem*> selected = ui->layer_list->selectedItems();
    if( selected.size() )
        return static_cast<CLayer*>(ui->layer_list->itemWidget(selected.front()));
    else
        return nullptr;
}

void ProjTab::slot_select_changed()
{
    QList<QListWidgetItem*> selected = ui->layer_list->selectedItems();
    if (selected.count())
    {
        ui->down->setEnabled(true);
        ui->up->setEnabled(true);
    }
    else
    {
        ui->down->setEnabled(false);
        ui->up->setEnabled(false);
    }
    if(selected.size() == 1)
        m_preview = true;
    else
        m_preview = false;
    emit sig_can_change_preview(m_id, m_preview);
}

bool ProjTab::have_undo()
{
    return m_proj_v.haveUndo();
}
bool ProjTab::have_redo()
{
    return m_proj_v.haveRedo();
}

void ProjTab::count_vars()
{
    m_number = 0;
    m_number = m_proj.count();
}

void ProjTab::task(std::vector<QString> & images, std::vector<Data::CColor *> & colors)
{
    images.resize(ui->layer_list->count());
    colors.resize(ui->layer_list->count());
    for (int i = 0; i < ui->layer_list->count(); ++i )
    {
        CLayer* layer = static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->item(i)));
        const QString *temp_ptr = layer->preview_get_file();
        if(temp_ptr == nullptr)
            continue;
        images[i] = *(temp_ptr);
        colors[i] = layer->preview_get_color();
    }
}

bool ProjTab::next_file()
{
    if (!m_preview) return false;
    CLayer* layer = static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()));
    if (layer->preview_have_file_next())
        layer->preview_file_next();
    return layer->preview_have_file_next();
}

bool ProjTab::prev_file()
{
    if (!m_preview) return false;
    CLayer* layer = static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()));
    if (layer->preview_have_file_prev())
        layer->preview_file_prev();
    return layer->preview_have_file_prev();
}

bool ProjTab::next_color()
{
    if (!m_preview) return false;
    CLayer* layer = static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()));
    if (layer->preview_have_color_next())
        layer->preview_color_next();
    return layer->preview_have_color_next();
}

bool ProjTab::prev_color()
{
    if (!m_preview) return false;
    CLayer* layer = static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()));
    if (layer->preview_have_color_prev())
        layer->preview_color_prev();
    return layer->preview_have_color_prev();
}

bool ProjTab::have_next_file()
    { return static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()))->preview_have_file_next(); }
bool ProjTab::have_prev_file()
    { return static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()))->preview_have_file_prev(); }
bool ProjTab::have_next_color()
    { return static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()))->preview_have_color_next(); }
bool ProjTab::have_prev_color()
    { return static_cast<CLayer*>(ui->layer_list->itemWidget(ui->layer_list->selectedItems().front()))->preview_have_color_prev(); }

