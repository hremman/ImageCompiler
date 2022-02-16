#include "CompilerMainWin.h"
#include "ui_CompilerMainWin.h"

#include <QPixmap>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include "ProjTab.h"

QImage img;

CompilerMainWin::CompilerMainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabs->removeTab(1);
    ui->tabs->addTab(new ProjTab, "пустой проект");


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

