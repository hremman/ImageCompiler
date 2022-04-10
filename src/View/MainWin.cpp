#include "MainWin.h"
#include "ui_MainWin.h"

#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFileDialog>


#include "View/dialogprogress.h"

QImage img;

CompilerMainWin::CompilerMainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UiMainWin)
    , m_scene(this)
    , m_item()
    , m_projects()
    , m_tabs_index()
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/icon/resourses/App.ico"));
    setWindowTitle("Компилятор изображений");
    ui->tabs->clear();

    ui->label_curr->setText("0");
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










    /*QImage img_1, img_2;
    img_1.load("h:\\mix_r.png");
    img_2.load("h:\\mix_b.png");
    img.~QImage();
    new (&img) QImage(img_1.width(),img_1.height(),img_1.format());
    for (auto col = 0; col < img_1.width(); col++)
    {
        for (auto row = 0; row < img_1.height(); row++)
        {
            QColor p1 = img_1.pixelColor(col, row);
            QColor p2 = img_2.pixelColor(col, row);
            QColor p_res;
            if ( p2.alpha() == 0 && p1.alpha() == 0 )
            {
                p_res.setRgba(qRgba(255,255,255,0));
                img.setPixelColor(col, row, p_res);
                continue;
            }
            if ( p2.alpha() == 0 )
            {
                img.setPixelColor(col, row, p1);
                continue;
            }
            if ( p1.alpha() == 0 )
            {
                img.setPixelColor(col, row, p2);
                continue;
            }
            p_res.setAlphaF( p1.alphaF() + p2.alphaF()*(1-p1.alphaF()) );
            //RRGB = (SRGB×SA + DRGB×DA×(1 − SA)) / RA
            p_res.setRed((p1.red()*p1.alphaF() + p2.red()*p2.alphaF()*(1-p1.alphaF()))/p_res.alphaF() );
            p_res.setGreen((p1.green()*p1.alphaF() + p2.green()*p2.alphaF()*(1-p1.alphaF()))/p_res.alphaF());
            p_res.setBlue((p1.blue()*p1.alphaF() + p2.blue()*p2.alphaF()*(1-p1.alphaF()))/p_res.alphaF());


            img.setPixelColor(col, row, p_res);

        }
    }

    ui->show_img->setScene(&m_scene);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(img.scaled(ui->show_img->width(), ui->show_img->height(), Qt::KeepAspectRatio)));
    m_scene.addItem(item);
    ui->show_img->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->show_img->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->show_img->update();
*/
}

void CompilerMainWin::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    /*m_scene.clear();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(img.scaled(ui->show_img->width(), ui->show_img->height(), Qt::KeepAspectRatio)));
    m_scene.addItem(item);
    m_scene.setSceneRect(m_scene.itemsBoundingRect());
    ui->show_img->update();*/

}

CompilerMainWin::~CompilerMainWin()
{
    delete ui;
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

    QObject::connect(temp_tab, &ProjTab::changed, this, &CompilerMainWin::slot_tab_edited);
    QObject::connect(temp_tab, &ProjTab::project_renamed, this, &CompilerMainWin::slot_tab_renamed);

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
    load(temp_proj);

    ProjTab *temp_tab = new ProjTab(temp_proj);
    m_proj_index.insert(temp_tab->getId(), it);
    m_tabs_index.insert(temp_tab->getId(), temp_tab);
    m_tabs_edited.insert(temp_tab->getId(), false);
    int index = ui->tabs->addTab(temp_tab, temp_proj.m_name);
    ui->tabs->setCurrentIndex(index);

    QObject::connect(temp_tab, &ProjTab::changed, this, &CompilerMainWin::slot_tab_edited);
    QObject::connect(temp_tab, &ProjTab::project_renamed, this, &CompilerMainWin::slot_tab_renamed);

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
        ui->label_curr->setText("0");
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
        QDir::setCurrent(m_proj_index[id]->m_file);
    if (m_tabs_index[id]->have_redo())
        ui->toolbar_redo->setEnabled(true);
    else
        ui->toolbar_redo->setEnabled(false);
    if (m_tabs_index[id]->have_undo())
        ui->toolbar_undo->setEnabled(true);
    else
        ui->toolbar_undo->setEnabled(false);
    show_counters();

}

void CompilerMainWin::save(const Data::CProject &data) const
{
    nlohmann::json out = data.to_json();
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
    //auto raw = nlohmann::json::from_bson();
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
    data.from_jsom(nlohmann::json::parse(bson));
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

void CompilerMainWin::slot_tab_renamed(unsigned int id)
{
    auto temp_tab = m_tabs_index[id];
    int i = 0;
    for (  ; i < ui->tabs->count(); i++)
    {
        if (ui->tabs->widget(i) == temp_tab)
            ui->tabs->setTabText(i, m_proj_index[id]->m_name);
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
    DialogProgress dialog(this);
    dialog.exec();
}
