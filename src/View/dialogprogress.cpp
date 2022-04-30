
#include "dialogprogress.h"
#include "ui_dialogprogress.h"



DialogProgress::DialogProgress(Data::CProject *proj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProgress),
    m_wait(":/image/icon/resourses/wait.ico"),
    m_done(":/image/icon/resourses/Ok.ico"),
    m_error(":/image/icon/resourses/cross.ico"),
    m_warn(":/image/icon/resourses/warn.ico"),
    m_pending(":/image/animation/resourses/pending.gif"),
    m_compiler(),
    m_paused(false),
    m_started(false),
    m_proj(proj)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0, 25);
    ui->tableWidget->setColumnWidth(1, 422);
    m_pending.setScaledSize(QSize(17,17));
    m_wait.scaledToWidth(23);
    m_done.scaledToWidth(23);
    m_error.scaledToWidth(23);

    for (int it = static_cast<int>(CCompiler::Event::Stage::FIRST); it < static_cast<int>(CCompiler::Event::Stage::COUNT); ++it)
        add_line(CCompiler::Event::stages()[it]);

    QObject::connect(ui->Cancel, &QPushButton::clicked, this, &DialogProgress::slot_cancel);

    QObject::connect(ui->pause_start, &QPushButton::clicked, this, &DialogProgress::slot_pause_start);
    QObject::connect(&m_compiler, &CCompiler::sendEvent, this, &DialogProgress::slot_processorEvent);


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
    ui->tableWidget->cellWidget(ui->tableWidget->rowCount() - 1, 0 )->setToolTip("Ожидание запуска");
}

void DialogProgress::set_done(CCompiler::Event::Stage stage)
{
    int line = static_cast<int>(stage);
    QLabel *l = new QLabel();
    l->setPixmap(m_done);
    l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setCellWidget(line, 0,l);
    ui->tableWidget->cellWidget(line, 0 )->setToolTip("Выполнено");
}

void DialogProgress::set_in_work(CCompiler::Event::Stage stage)
{
    int line = static_cast<int>(stage);
    if ( m_pending.state() != QMovie::Running )
        m_pending.start();

    QLabel *l = new QLabel();
    l->setMovie(&m_pending);
    l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setCellWidget(line, 0,l);
    ui->tableWidget->cellWidget(line, 0 )->setToolTip("Выполняется");
}

void DialogProgress::set_error(CCompiler::Event::Stage stage, const QString & text)
{
    int line = static_cast<int>(stage);
    QLabel *l = new QLabel();
    l->setPixmap(m_error);
    l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setCellWidget(line, 0,l);
    ui->tableWidget->cellWidget(line, 1 )->setToolTip(text);
    ui->tableWidget->cellWidget(line, 0 )->setToolTip("Ошибка");
}

void DialogProgress::set_warn(CCompiler::Event::Stage stage, const QString & text)
{

    int line = static_cast<int>(stage);
    QLabel *l = new QLabel();
    l->setPixmap(m_warn);
    l->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->tableWidget->setCellWidget(line, 0,l);
    ui->tableWidget->cellWidget(line, 1 )->setToolTip(text);
    ui->tableWidget->cellWidget(line, 0 )->setToolTip("Предупреждение");

}

void DialogProgress::showEvent(QShowEvent *event)
{
    m_lock.lock();
    //Извлечь события процессора и обработать их
    std::list<CCompiler::Event> local_que(std::move(m_event_que));
    m_lock.unlock();
    for (auto it = local_que.begin(); it != local_que.end(); ++it)
    {
        switch (it->type())
        {
            case CCompiler::Event::Type::OK:
                set_done(it->stage());
                if (it->stage() == CCompiler::Event::Stage::LAST)
                    ui->progressBar->setHidden(true);
            break;

            case CCompiler::Event::Type::INWORK:
                set_in_work(it->stage());
                if (it->stage() == CCompiler::Event::Stage::FIRST)
                    ui->progressBar->setHidden(false);
                break;

            case CCompiler::Event::Type::WARNING:
                set_warn(it->stage(), it->message());
                break;

            case CCompiler::Event::Type::ERROR:
                ui->progressBar->setHidden(true);
                set_error(it->stage(), it->message());
                break;

            case CCompiler::Event::Type::ONPROGRESS:
                ui->progressBar->setMaximum(it->progress().first);
                ui->progressBar->setValue(it->progress().second);
                break;
            default:
                break;
        }
    }

    QDialog::showEvent(event);
}

void DialogProgress::slot_cancel(bool)
{
    m_compiler.stop();
}

void DialogProgress::slot_pause_start(bool)
{
    if ( !m_started )
    {
        m_compiler.build(*m_proj);// TODO ЗАпустить в QtConcurent
        ui->pause_start->setText("Пауза");
        m_paused = false;
        return;
    }
    if (m_paused)
    {
        ui->pause_start->setText("Пауза");
        m_paused = false;
        m_compiler.resume();
    }
    else
    {
        ui->pause_start->setText("Продолжить");
        m_paused = true;
        m_compiler.pause();
    }
}

void DialogProgress::slot_processorEvent(CCompiler::Event event)
{
    m_lock.lock();
    m_event_que.push_back(event);
    m_lock.unlock();
}
