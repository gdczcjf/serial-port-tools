#include "wave_view.h"
#include <QPainter>

static const int WIDTH = 540;
static const int HEIGHT = 280;

WaveView::WaveView(QWidget *parent) :
    QWidget(parent)
{
    this->setMouseTracking(true);
    this->setCursor(Qt::CrossCursor);
    th=0;
    frame.fill(0,128);
    /*
    for(int i = 0; i < 128; ++i)
    {
        frame[i] = i * 2;
    }*/
}

void WaveView::paintEvent(QPaintEvent *)
{

    QPainter painter(this);

    painter.setWindow(0,0,WIDTH,HEIGHT);
    painter.fillRect(0,0,WIDTH,HEIGHT,QColor(0,0,0));
    drawCoordinate(painter);
    painter.setPen(QColor(Qt::yellow));
    int lastIndex = frame.size()-1;
    for(int i = 0; i< lastIndex; ++i)
    {
        painter.drawLine(transPoint(i * 4, (unsigned char)frame.at(i)) , transPoint((i+1)*4, (unsigned char)frame.at(i)));
        painter.drawLine(transPoint((i+1) * 4, (unsigned char)frame.at(i)) , transPoint((i+1) * 4, (unsigned char)frame.at(i+1)));
    }
    painter.drawLine(transPoint(lastIndex * 4, (unsigned char)frame.at(lastIndex)) , transPoint((lastIndex+1)*4, (unsigned char)frame.at(lastIndex)));
    painter.setPen(QColor(Qt::cyan));
    painter.drawLine(transPoint(0,th),transPoint((lastIndex+1)*4,th));
}

QPoint WaveView::transPoint(int x, int y)
{
    return QPoint(x + WIDTH - 512 - 10, 255 - y + 10);
}

QPoint WaveView::reTransPoint(int x, int y)
{
    return QPoint(x - WIDTH + 512 + 10, 255 - y + 10);
}

void WaveView::disp(const QByteArray &dat,unsigned char th)
{
    if(dat.size() != 128)
      return;
    frame = dat;
    this->th = th;
    this->repaint();
}

void WaveView::drawCoordinate(QPainter &painter)
{
    painter.setPen(QColor(Qt::darkGreen));
    for(int i = 50; i < 250; i+=50)
    {
        painter.drawLine(transPoint(0,i),transPoint(128*4,i));
        painter.drawText(QRect(transPoint(0-17,i+5),transPoint(0,i-5)),Qt::AlignRight,QString::number(i));
    }

    for(int i = 10; i < 128; i+=10)
    {
        painter.drawLine(transPoint(i*4,0),transPoint(i*4,255));
        painter.drawText(QRect(transPoint(i*4-9,0),transPoint(i*4+9,-10)),Qt::AlignHCenter,QString::number(i));
    }
    painter.drawLine(transPoint(0,0),transPoint(128*4,0));
    painter.drawText(QRect(transPoint(-22,0),transPoint(-3,-10)),Qt::AlignRight,QString::number(0));

    painter.drawLine(transPoint(0,255),transPoint(128*4,255));
    painter.drawText(QRect(transPoint(0-17,255+5),transPoint(0,255-5)),Qt::AlignRight,QString::number(255));
    painter.drawLine(transPoint(0,0),transPoint(0,255));
    painter.drawLine(transPoint(128*4,0),transPoint(128*4,255));
    painter.drawText(QRect(transPoint(522-18,0),transPoint(522,-10)),Qt::AlignHCenter,QString::number(128));
}

void WaveView::mouseMoveEvent ( QMouseEvent *event)
{
    int x = (double)event->x() * (double)WIDTH / (double)width();
    int y = (double)event->y() * (double)HEIGHT / (double)height();
    QPoint pos = reTransPoint(x,y);
    pos.setX(pos.x() / 4);
    emit(mouseMove(pos));
}
