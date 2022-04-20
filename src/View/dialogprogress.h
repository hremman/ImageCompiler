#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QMovie>
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
    explicit DialogProgress(QWidget *parent = nullptr);
    ~DialogProgress();

public slots:
    void slot_clicked(bool);
    void slot_processorEvent(CCompiler::Event);

private:
    Ui::DialogProgress *ui;
    int m_current_line;
    QPixmap m_wait;
    QPixmap m_done;
    QPixmap m_error;
    QPixmap m_warn;
    QMovie m_pending;
    CCompiler & m_processor;
    CSpinLock m_lock;
    std::list<CCompiler::Event> m_event_que;


protected:
    virtual void showEvent(QShowEvent *event) override;
private:
    void add_line(const QString &);

    void set_in_work();
    void set_error(const QString &);
    void set_warn(const QString &);
};

#endif // DIALOGPROGRESS_H
