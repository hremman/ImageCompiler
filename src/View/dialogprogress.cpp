
#include "dialogprogress.h"
#include "ui_dialogprogress.h"


DialogProgress::DialogProgress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress),
    m_current_line(-1),
    m_wait(":/image/icon/resourses/wait.ico"),
    m_done(":/image/icon/resourses/Ok.ico"),
    m_error(":/image/icon/resourses/cross.ico"),
    m_warn(":/image/icon/resourses/warn.ico"),
    m_pending(":/image/animation/resourses/pending.gif"),
    m_processor(CCompiler::getProcessor())
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, 25);
    ui->tableWidget->setColumnWidth(1, 422);
    m_pending.setScaledSize(QSize(17,17));
    m_wait.scaledToWidth(23);
    m_done.scaledToWidth(23);
    m_error.scaledToWidth(23);

    add_line("Анализ проекта");
    add_line("Генерация цветов");
    add_line("Проверка вероятностей");
    add_line("Проверка изображений");
    add_line("Построение кэшей");
    add_line("Подготовка заданий");
    add_line("Генерация изображений");

    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &DialogProgress::slot_clicked);
    QObject::connect(&m_processor, &CCompiler::sendEvent, this, &DialogProgress::slot_processorEvent);


}

DialogProgress::~DialogProgress()
{
    delete ui;
}


void DialogProgress::add_line(const QString & text)
{
    QLabel *l = new QLabel();
    l->setPixmap(m_wait);
    l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1,0,l);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount() - 1,1,new QLabel(text));
}

void DialogProgress::set_in_work()
{
    if (m_current_line >= 0)
    {
        m_pending.stop();
        QLabel *l = new QLabel();
        l->setPixmap(m_done);
        l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setCellWidget(m_current_line, 0,l);
    }

    if (m_current_line < ui->tableWidget->rowCount() - 1)
    {
        QLabel *l = new QLabel();
        l->setMovie(&m_pending);
        l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setCellWidget(++m_current_line, 0,l);
        m_pending.start();
    }


}


void DialogProgress::showEvent(QShowEvent *event)
{
    m_lock.lock();
    //Извлечь события процессора и обработать их


    m_lock.unlock();
    QDialog::showEvent(event);
}


void DialogProgress::set_error(const QString & text)
{
    if (m_current_line >= 0)
    {
        m_pending.stop();
        QLabel *l = new QLabel();
        l->setPixmap(m_error);
        l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setCellWidget(m_current_line, 0,l);
        ui->tableWidget->cellWidget(m_current_line, 1 )->setToolTip(text);
    }
}

void DialogProgress::set_warn(const QString & text)
{

    if (m_current_line >= 0)
    {
        m_pending.stop();
        QLabel *l = new QLabel();
        l->setPixmap(m_warn);
        l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setCellWidget(m_current_line, 0,l);
        ui->tableWidget->cellWidget(m_current_line, 1 )->setToolTip(text);
    }
    if (m_current_line < ui->tableWidget->rowCount() - 1)
    {
        QLabel *l = new QLabel();
        l->setMovie(&m_pending);
        l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        ui->tableWidget->setCellWidget(++m_current_line, 0,l);
        m_pending.start();
    }

}



void DialogProgress::slot_clicked(bool)
{
}

void DialogProgress::slot_processorEvent(CCompiler::Event event)
{
    m_lock.lock();
    m_event_que.push_back(event);
    m_lock.unlock();
}
