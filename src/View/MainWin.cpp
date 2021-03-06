#include "MainWin.h"
#include "ui_MainWin.h"
#include <QFileInfo>
#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>
#include "Algo/PreviewBuilder.hpp"

#include "Exceptions.hpp"

#include "View/dialogprogress.h"
#include "View/Layer.h"

CompilerMainWin::CompilerMainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UiMainWin)
    , m_scene(this)
    , m_item()
    , m_projects()
    , m_tabs_index()
    , m_watchdog()
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/icon/resourses/App.ico"));
    setWindowTitle("Компилятор изображений");
    ui->tabs->clear();




    ui->label_outof->setText("0");


    QObject::connect(ui->menu_create, &QAction::triggered, this, &CompilerMainWin::slot_create);
    QObject::connect(ui->toolbar_create, &QAction::triggered, this, &CompilerMainWin::slot_create);

    QObject::connect(ui->menu_open, &QAction::triggered, this, &CompilerMainWin::slot_open);
    QObject::connect(ui->toolbar_open, &QAction::triggered, this, &CompilerMainWin::slot_open);

    QObject::connect(ui->menu_save, &QAction::triggered, this, &CompilerMainWin::slot_save);
    QObject::connect(ui->toolbar_save, &QAction::triggered, this, &CompilerMainWin::slot_save);

    QObject::connect(ui->menu_save_all, &QAction::triggered, this, &CompilerMainWin::slot_save_all);
    QObject::connect(ui->toolbar_save_all, &QAction::triggered, this, &CompilerMainWin::slot_save_all);

    QObject::connect(ui->menu_save_copy, &QAction::triggered, this, &CompilerMainWin::slot_save_copy);

    QObject::connect(ui->menu_close, &QAction::triggered, this, &CompilerMainWin::slot_close);

    QObject::connect(ui->menu_close_all, &QAction::triggered, this, &CompilerMainWin::slot_close_all);
    QObject::connect(ui->toolbar_close_all, &QAction::triggered, this, &CompilerMainWin::slot_close_all);

    QObject::connect(ui->menu_exit, &QAction::triggered, this, &CompilerMainWin::slot_exit);

    QObject::connect(ui->tabs, &QTabWidget::tabCloseRequested, this, &CompilerMainWin::slot_close_tab);
    QObject::connect(ui->tabs, &QTabWidget::currentChanged, this, &CompilerMainWin::slot_tab_changed);

    QObject::connect(ui->toolbar_redo, &QAction::triggered, this, &CompilerMainWin::slot_redo);
    QObject::connect(ui->toolbar_undo, &QAction::triggered, this, &CompilerMainWin::slot_undo);

    QObject::connect(ui->do_it, &QPushButton::clicked, this, &CompilerMainWin::slot_generate);

    QObject::connect(ui->next_color, &QToolButton::clicked, this, &CompilerMainWin::slot_next_color);
    QObject::connect(ui->prev_color, &QToolButton::clicked, this, &CompilerMainWin::slot_prev_color);
    QObject::connect(ui->next_file, &QToolButton::clicked, this, &CompilerMainWin::slot_next_file);
    QObject::connect(ui->prev_file, &QToolButton::clicked, this, &CompilerMainWin::slot_prev_file);

    QObject::connect(ui->preview_enable, &QCheckBox::toggled, this, &CompilerMainWin::slot_preview_allowed);


    ui->menu_save->setEnabled(false);
    ui->toolbar_save->setEnabled(false);
    ui->menu_save_all->setEnabled(false);
    ui->toolbar_save_all->setEnabled(false);
    ui->menu_save_copy->setEnabled(false);
    ui->toolbar_close_all->setEnabled(false);
    ui->menu_close_all->setEnabled(false);
    ui->menu_close->setEnabled(false);
    ui->toolbar_redo->setEnabled(false);
    ui->toolbar_undo->setEnabled(false);
    ui->preview_progress->setVisible(false);
    QDir::setCurrent(QDir::homePath());

    switch_preview_buttons(false);

    ui->graphicsView->setScene(&m_scene);


}

void CompilerMainWin::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if ( ui->tabs->count())
        setPreview( static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex())) );
}

CompilerMainWin::~CompilerMainWin()
{
    delete ui;
}

void CompilerMainWin::switch_preview_buttons(bool val)
{
    if ( !ui->preview_enable->isChecked() || !ui->tabs->count() )
    {
        ui->prev_color->setEnabled(false);
        ui->next_color->setEnabled(false);
        ui->next_file->setEnabled(false);
        ui->prev_file->setEnabled(false);
        return;
    }

    ProjTab *temp_tab = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    const CLayer *temp_layer = temp_tab->get_single_selected();

    if ( temp_layer && temp_layer->preview_have_color_next() )
        ui->next_color->setEnabled(val);
    else
        ui->next_color->setEnabled(false);

    if ( temp_layer && temp_layer->preview_have_color_prev() )
        ui->prev_color->setEnabled(val);
    else
        ui->prev_color->setEnabled(false);

    if ( temp_layer && temp_layer->preview_have_file_next() )
        ui->next_file->setEnabled(val);
    else
        ui->next_file->setEnabled(false);

    if ( temp_layer && temp_layer->preview_have_file_prev() )
        ui->prev_file->setEnabled(val);
    else
        ui->prev_file->setEnabled(false);
}

void CompilerMainWin::slot_create(bool)
{
    m_projects.emplace_front();

    ProjTab *temp_tab = new ProjTab(m_projects.front());
    m_proj_index.insert(temp_tab->getId(), m_projects.begin());
    m_tabs_index.insert(temp_tab->getId(), temp_tab);
    m_tabs_edited.insert(temp_tab->getId(), false);
    int index = ui->tabs->addTab(temp_tab, "Новый проект");
    ui->tabs->setCurrentIndex(index);

    QObject::connect(temp_tab, &ProjTab::sig_changed, this, &CompilerMainWin::slot_tab_edited);
    QObject::connect(temp_tab, &ProjTab::sig_project_renamed, this, &CompilerMainWin::slot_tab_renamed);
    QObject::connect(temp_tab, &ProjTab::sig_can_change_preview, this, &CompilerMainWin::slot_enable_preview_button);

    ui->menu_close->setEnabled(true);
    ui->menu_close_all->setEnabled(true);
    ui->toolbar_close_all->setEnabled(true);
    ui->menu_save_copy->setEnabled(true);
}

void CompilerMainWin::slot_open(bool){
    QString file = QFileDialog::getOpenFileName(this, "Открыть проект", "./", "Файл проекта (*.icproj)");
    if (file.isEmpty())
        return;
    m_projects.emplace_front();
    Data::CProject &temp_proj = m_projects.front();
    auto it = m_projects.begin();
    temp_proj.m_file = file;
    try
        {load(temp_proj);}
    catch (const  ICException& e)
    {
        QMessageBox::critical(this, "Невозможно открыть проект", "Возникла неустранимая ошибка:\n" + e.message() + "\nПроект не будет загружен.");
        m_projects.pop_front();
        return;
    }
    catch (const  nlohmann::json::exception & e)
    {
        QMessageBox::critical(this, "Невозможно открыть проект", "Возникла неустранимая ошибка:\n" + QString(e.what()) + "\nПроект не будет загружен.");
        m_projects.pop_front();
        return;
    }

    QString old_path = QDir::currentPath();
    QDir::setCurrent(QFileInfo(temp_proj.m_file).absolutePath());

    const std::list<Data::CLayer*> &layers = temp_proj.layers();

    std::set<int> hashes;
    QStringList errored;
    QStringList errors;

    for (Data::CLayer* layer: layers)
    {
        for (QString image_file: layer->m_files)
        {
            QFile test;
            test.setFileName(image_file);
            if ( !test.open(QIODeviceBase::ReadOnly) )
            {
                size_t hash = qHash(image_file);
                if ( hashes.find(hash) == hashes.end() )
                {
                    errored.append(image_file);
                    errors.append(test.errorString());
                    hashes.insert(hash);
                }

            }
            else
                test.close();
        }
    }
    QString text = "Не удалось получить доступ к фалам ниже. Убрать их из проекта?";

    auto errored_ = errored.begin();
    auto errors_ = errors.begin();
    for (; errored_ != errored.end() && errors_ != errors.end() ; ++errored_, ++errors_ )
        text += "\n\t" + QFileInfo(*errored_).absolutePath() + " [" + *errors_ + "]";

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Файлы не достижимы", text);

    if (reply == QMessageBox::Yes)
    {
        for (Data::CLayer* layer: layers)
        {
            QMutableListIterator<QString> file(layer->m_files);
            while (file.hasNext()) {
                file.next();
                if ( hashes.find(qHash(file.value())) != hashes.end() )
                    file.remove();
            }
        }
    }


    ProjTab *temp_tab = nullptr;
    try {
        temp_tab = new ProjTab(temp_proj);
    }  catch (const too_many_id &e)
    {
        QMessageBox::critical(this, "Невозможно отобразить проект", "Возникла неустранимая ошибка:\n" + e.message() + "\nПроект будет закрыт. Следует закрыть хотя бы 1 открытый проект перед повторной попыткой");
        m_projects.pop_front();
        delete temp_tab;
        QDir::setCurrent(old_path);
        return;
    }

    m_proj_index.insert(temp_tab->getId(), it);
    m_tabs_index.insert(temp_tab->getId(), temp_tab);
    m_tabs_edited.insert(temp_tab->getId(), false);
    int index = ui->tabs->addTab(temp_tab, temp_proj.m_name);
    ui->tabs->setCurrentIndex(index);

    QObject::connect(temp_tab, &ProjTab::sig_changed, this, &CompilerMainWin::slot_tab_edited);
    QObject::connect(temp_tab, &ProjTab::sig_project_renamed, this, &CompilerMainWin::slot_tab_renamed);
    QObject::connect(temp_tab, &ProjTab::sig_can_change_preview, this, &CompilerMainWin::slot_enable_preview_button);


    ui->menu_close->setEnabled(true);
    ui->menu_close_all->setEnabled(true);
    ui->toolbar_close_all->setEnabled(true);
    ui->menu_save_copy->setEnabled(true);


}

void CompilerMainWin::slot_save(bool)
{
    unsigned int id = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()))->getId();
    if (m_tabs_edited[id]== true)
        return;
    Data::CProject & temp_proj = *m_proj_index[id];
    temp_proj = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()))->getLast();
    if (temp_proj.m_file.isEmpty())
    {
        temp_proj.m_file = QFileDialog::getSaveFileName(this, "Сохранить как...", "./", "Файл проекта (*.icproj)");
        if (temp_proj.m_file.isEmpty())
            return;
    }

    save(temp_proj);
    m_tabs_edited[id]=false;
    ui->toolbar_save->setEnabled(false);
    ui->menu_save->setEnabled(false);
    bool val = false;
    for ( auto it = m_tabs_edited.begin(); it != m_tabs_edited.end(); it++)
        if ( it.value() )
        {
            val = true;
            break;
        }
    ui->menu_save_all->setEnabled(val);
    ui->toolbar_save_all->setEnabled(val);
}

void CompilerMainWin::slot_save_all(bool){
    for (int it = 0; it < ui->tabs->count(); it++)
    {
        ui->tabs->setCurrentIndex(it);
        slot_save(true);
    }
    ui->menu_save_all->setEnabled(false);
    ui->toolbar_save_all->setEnabled(false);
}

void CompilerMainWin::slot_save_copy(bool)
{
    unsigned int id = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()))->getId();
    Data::CProject & temp_proj = *m_proj_index[id];
    temp_proj = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()))->getLast();
    QString title;
    if (temp_proj.m_file.isEmpty())
        title = "Сохранить как...";
    else
        title = "Сохранить копию как...";

    QString file = QFileDialog::getSaveFileName(this, title, "./", "Файл проекта (*.icproj)");
    if (file.isEmpty())
        return;
    temp_proj.m_file = file;
    save(temp_proj);
    m_tabs_edited[id]=false;
    ui->toolbar_save->setEnabled(false);
    ui->menu_save->setEnabled(false);
    bool val = false;
    for ( auto it = m_tabs_edited.begin(); it != m_tabs_edited.end(); it++)
        if ( it.value() )
        {
            val = true;
            break;
        }
    ui->menu_save_all->setEnabled(val);
    ui->toolbar_save_all->setEnabled(val);


}

void CompilerMainWin::slot_close(bool)
{
    slot_close_tab(ui->tabs->currentIndex());
}

void CompilerMainWin::slot_close_all(bool)
{
    for (int it = 0; it < ui->tabs->count(); it++)
    {
        slot_close(it);
    }
}

void CompilerMainWin::slot_exit(bool){
    slot_close_all(true);
    QApplication::quit();

}

void CompilerMainWin::slot_close_tab(int index)
{
    ProjTab* temp_tab = static_cast<ProjTab*>(ui->tabs->widget(index));
    unsigned int id = temp_tab->getId();
    if (m_tabs_edited[id])
    {
        QMessageBox dialog(this);
        dialog.setWindowTitle("Закрытие проекта");
        QString text = "Проект " + static_cast<ProjTab*>(ui->tabs->widget(index))->getLast().m_name + " не сохранён. Сохранить перед закрытием?";
        dialog.setText(text);
        QPushButton * yes = dialog.addButton("Да", QMessageBox::ButtonRole::YesRole);
        QPushButton * no = dialog.addButton("Нет", QMessageBox::ButtonRole::NoRole);
        dialog.addButton("Не закрывать", QMessageBox::ButtonRole::RejectRole);
        dialog.setDefaultButton(yes);

        dialog.exec();

        if (dialog.clickedButton() == yes)
        {
            Data::CProject & temp_proj = *m_proj_index[id];
            temp_proj = temp_tab->getLast();
            if (temp_proj.m_file.isEmpty())
            {
                temp_proj.m_file = QFileDialog::getSaveFileName(this, "Сохранить как...", "./", "Файл проекта (*.icproj)");
                if (temp_proj.m_file.isEmpty())
                    return;
            }

            save(temp_proj);

            m_projects.erase(m_proj_index[id]);
            m_proj_index.remove(id);
            m_tabs_index.remove(id);
            m_tabs_edited.remove(id);
            delete temp_tab;
            ui->tabs->removeTab(index);
        }
        else if (dialog.clickedButton() == no)
        {
            m_projects.erase(m_proj_index[id]);
            m_proj_index.remove(id);
            m_tabs_index.remove(id);
            m_tabs_edited.remove(id);
            delete temp_tab;
            ui->tabs->removeTab(index);

        }
    }
    else
    {
        m_projects.erase(m_proj_index[id]);
        m_proj_index.remove(id);
        m_tabs_index.remove(id);
        m_tabs_edited.remove(id);
        delete temp_tab;
        ui->tabs->removeTab(index);
    }

    if ( m_projects.size() == 0)
    {
        ui->menu_close->setEnabled(false);
        ui->menu_close_all->setEnabled(false);
        ui->toolbar_close_all->setEnabled(false);
        ui->toolbar_save->setEnabled(false);
        ui->menu_save->setEnabled(false);
        ui->menu_save_all->setEnabled(false);
        ui->toolbar_save_all->setEnabled(false);
        ui->menu_save_copy->setEnabled(false);
        ui->label_outof->setText("0");
        ui->toolbar_undo->setEnabled(false);
        ui->toolbar_redo->setEnabled(false);
    }
    else
    {
        bool val = false;
        for ( auto it = m_tabs_edited.begin(); it != m_tabs_edited.end(); it++)
            if ( it.value() )
            {
                val = true;
                break;
            }
        ui->menu_save_all->setEnabled(val);
        ui->toolbar_save_all->setEnabled(val);
    }


}

void CompilerMainWin::slot_tab_edited(unsigned int id)
{
    m_tabs_edited[id] = true;
    if ( ui->tabs->widget(ui->tabs->currentIndex()) == m_tabs_index[id] )
    {
        ui->toolbar_save->setEnabled(true);
        ui->menu_save->setEnabled(true);
        if (m_tabs_index[id]->have_redo())
            ui->toolbar_redo->setEnabled(true);
        else
            ui->toolbar_redo->setEnabled(false);
        if (m_tabs_index[id]->have_undo())
            ui->toolbar_undo->setEnabled(true);
        else
            ui->toolbar_undo->setEnabled(false);
    }
    ui->toolbar_save_all->setEnabled(true);
    ui->menu_save_all->setEnabled(true);
    show_counters();
    setPreview(m_tabs_index[id]);
}

void CompilerMainWin::slot_tab_changed(int index)
{
    if (index == -1)
        return;
    auto id = static_cast<ProjTab*>(ui->tabs->widget(index))->getId();
    if ( m_tabs_edited[id] )
    {
        ui->toolbar_save->setEnabled(true);
        ui->menu_save->setEnabled(true);
    }
    else
    {
        ui->toolbar_save->setEnabled(false);
        ui->menu_save->setEnabled(false);
    }
    if (ui->tabs->count() == 0)
        return;
    if ( ! m_proj_index[id]->m_file.isEmpty() )
        QDir::setCurrent(QFileInfo(m_proj_index[id]->m_file).absolutePath());
    if (m_tabs_index[id]->have_redo())
        ui->toolbar_redo->setEnabled(true);
    else
        ui->toolbar_redo->setEnabled(false);
    if (m_tabs_index[id]->have_undo())
        ui->toolbar_undo->setEnabled(true);
    else
        ui->toolbar_undo->setEnabled(false);
    show_counters();
    setPreview(m_tabs_index[id]);

}

void CompilerMainWin::save(const Data::CProject &data) const
{
    nlohmann::json out = data.to_json(QFileInfo(data.m_file).absolutePath());
    QFile file(data.m_file);
    file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Truncate);
    std::string raw = out.dump();
   // auto raw = nlohmann::json::to_bson(out);
    file.write(reinterpret_cast<const char *>(raw.data()), raw.size());
    file.close();

}
void CompilerMainWin::load(Data::CProject & data)
{
    QFile file(data.m_file);
    file.open(QIODeviceBase::ReadOnly | QIODeviceBase::ExistingOnly);
    char buffer[1024];
    long long c = 0;
    std::vector<unsigned char> bson;
    while ((c = file.read(buffer, 1024)) != 0)
    {
        size_t old = bson.size();
        bson.resize(old + c);
        memcpy(bson.data()+old,buffer, c);
    }
    file.close();
    data.from_json(nlohmann::json::parse(bson));
    //data.from_jsom(nlohmann::json::from_bson(bson));
}

void CompilerMainWin::closeEvent (QCloseEvent *event)
{
    QMessageBox dialog(this);
    dialog.setWindowTitle("Закрытие приложения");
    QString text;
    if (m_projects.size())
        text = "Открыты " + QString::number(m_projects.size()) + " проектов. Закрыть приложение?";
    else
        text = "Закрыть приложение?";
    dialog.setText(text);
    QPushButton * yes = dialog.addButton("Закрыть", QMessageBox::ButtonRole::YesRole);
    dialog.addButton("Оставить открытым", QMessageBox::ButtonRole::NoRole);
    dialog.setDefaultButton(yes);

    dialog.exec();
    if (dialog.clickedButton() == yes)
    {
        event->accept();
        slot_exit(true);
    }
    else
        event->ignore();
}

void CompilerMainWin::slot_redo(bool)
{
    if (ui->tabs->count() == 0)
        return;
    ProjTab *temp = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    if (temp->have_redo())
        temp->do_redo(true);
    if (!temp->have_redo())
        ui->toolbar_redo->setEnabled(false);
    else
        ui->toolbar_redo->setEnabled(true);
    if (temp->have_undo())
        ui->toolbar_undo->setEnabled(true);
    else
        ui->toolbar_undo->setEnabled(false);
    show_counters(temp);
}



void CompilerMainWin::slot_undo(bool)
{
    if (ui->tabs->count() == 0)
        return;
    ProjTab *temp = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    if (temp->have_undo())
        temp->do_undo(true);
    if (!temp->have_undo())
        ui->toolbar_undo->setEnabled(false);
    else
        ui->toolbar_undo->setEnabled(true);
    if (temp->have_redo())
        ui->toolbar_redo->setEnabled(true);
    else
        ui->toolbar_redo->setEnabled(false);
    show_counters(temp);
}

void CompilerMainWin::slot_tab_renamed(unsigned int id, const QString & name)
{
    auto temp_tab = m_tabs_index[id];
    int i = 0;
    for (  ; i < ui->tabs->count(); i++)
    {
        if (ui->tabs->widget(i) == temp_tab)
            ui->tabs->setTabText(i, name);
    }
}

void CompilerMainWin::show_counters(ProjTab *temp)
{
    if (temp == nullptr)
        temp = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    ui->label_outof->setText(QString::number(temp->m_number));
}

void CompilerMainWin::slot_generate(bool)
{

    DialogProgress dialog(&*(m_proj_index[static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()))->getId()]), this);
    dialog.exec();
}

void CompilerMainWin::slot_enable_preview_button(unsigned int id, bool can_be)
{
    if ( static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()))->getId() != id )
        return;
    switch_preview_buttons(can_be);
}

void CompilerMainWin::slot_next_color(bool)
{
    auto tab = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    ui->next_color->setEnabled(tab->next_color());
    ui->prev_color->setEnabled(tab->have_prev_color());
    build_preview();
}

void CompilerMainWin::slot_prev_color(bool)
{
    auto tab = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    ui->prev_color->setEnabled(tab->prev_color());
    ui->next_color->setEnabled(tab->have_next_color());
    build_preview();
}

void CompilerMainWin::slot_next_file(bool)
{
    auto tab = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    ui->next_file->setEnabled(tab->next_file());
    ui->prev_file->setEnabled(tab->have_prev_file());
    build_preview();
}

void CompilerMainWin::slot_prev_file(bool)
{
    auto tab = static_cast<ProjTab*>(ui->tabs->widget(ui->tabs->currentIndex()));
    ui->prev_file->setEnabled(tab->prev_file());
    ui->next_file->setEnabled(tab->have_next_file());
    build_preview();
}

void CompilerMainWin::slot_preview_finished()
{
    int current = ui->tabs->currentIndex();
    auto tab = static_cast<ProjTab*>(ui->tabs->widget(current));
    tab->m_preview_img = m_watchdog.result();
    QObject::disconnect(&m_watchdog, &QFutureWatcherBase::finished, this, &CompilerMainWin::slot_preview_finished );
    setPreview(tab);
    ui->preview_progress->setVisible(false);
    switch_preview_buttons(true);

    ui->tabs->setTabsClosable(true);
    for (int i = 0; i < ui->tabs->count() ; i++)
        if (i != current)
            ui->tabs->setTabEnabled(i, true);
}

void CompilerMainWin::setPreview(ProjTab* tab)
{
    auto items = m_scene.items();
    m_scene.clear();
    if ( tab->m_preview_img.isNull() )
        return;
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(tab->m_preview_img.scaled(ui->graphicsView->width(), ui->graphicsView->height(), Qt::KeepAspectRatio)));
    m_scene.addItem(item);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->update();
}

void CompilerMainWin::slot_preview_allowed(bool checked)
{
    switch_preview_buttons(checked);
    if (checked && ui->tabs->count())
        build_preview();
}

void CompilerMainWin::build_preview()
{
    ui->preview_progress->setVisible(true);
    int current = ui->tabs->currentIndex();
    auto tab = static_cast<ProjTab*>(ui->tabs->widget(current));
    ui->tabs->setTabsClosable(false);
    for (int i = 0; i < ui->tabs->count() ; i++)
        if (i != current)
            ui->tabs->setTabEnabled(i, false);
    switch_preview_buttons(false);

    std::vector<QString> images;
    std::vector<Data::CColor*> colors;
    tab->task(images, colors);
    m_watchdog.setFuture(QtConcurrent::run(CPreviewBuilder::buildPreview, std::move(images), std::move(colors)));
    QObject::connect(&m_watchdog, &QFutureWatcherBase::finished, this, &CompilerMainWin::slot_preview_finished );
}
