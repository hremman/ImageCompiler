#ifndef DIALOGPROGRESS_H
#define DIALOGPROGRESS_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QMovie>

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
    void clicked(bool);

private:
    Ui::DialogProgress *ui;
    int m_current_line;
    QPixmap m_wait;
    QPixmap m_done;
    QPixmap m_error;
    QPixmap m_warn;
    QMovie m_pending;

private:
    void add_line(const QString &);

    void set_in_work();
    void set_error(const QString &);
    void set_warn(const QString &);
};

#endif // DIALOGPROGRESS_H
