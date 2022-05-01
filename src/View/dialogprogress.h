#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QMovie>
#include <QFutureWatcher>
#include "Data/Project.hpp"
#include "Algo/Compiler.hpp"
#include "Algo/SpinLock.hpp"
#include "Algo/CompilerEvent.hpp"

namespace Ui {
class DialogProgress;
}

class DialogProgress : public QDialog
{
    Q_OBJECT

public:
    explicit DialogProgress(Data::CProject *, QWidget *parent = nullptr);
    ~DialogProgress();
    void closeEvent (QCloseEvent *event);

public slots:
    void slot_cancel(bool);
    void slot_close(bool);
    void slot_pause_start(bool);
    void slot_processorEvent(CCompiler::Event);
    void slot_finished();

private:
    Ui::DialogProgress *ui;
    QPixmap m_wait;
    QPixmap m_done;
    QPixmap m_error;
    QPixmap m_warn;
    QMovie m_pending;
    CCompiler m_compiler;
    CSpinLock m_lock;
    std::list<CCompiler::Event> m_event_que;
    bool m_paused;
    bool m_started;
    Data::CProject *m_proj;
    QFutureWatcher<bool> m_watchdog;


protected:
    virtual void showEvent(QShowEvent *event) override;
private:
    void add_line(const QString &);

    void set_done(CCompiler::Event::Stage);
    void set_in_work(CCompiler::Event::Stage);
    void set_error(CCompiler::Event::Stage, const QString &);
    void set_warn(CCompiler::Event::Stage, const QString &);

};

#endif // DIALOGPROGRESS_H
