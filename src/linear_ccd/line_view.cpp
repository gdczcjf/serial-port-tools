#include "line_view.h"
#include <QPainter>

static const int HEIGHT = 30;
static const int WIDTH  = 128;

GreyView::GreyView(QWidget *parent) :
    QWidget(parent)
{
    frame.fill(0,128);
}

void GreyView::disp(const QByteArray &dat)
{
    if(dat.size() != 128)
      return;
    frame = dat;
    this->repaint();
}

void GreyView::paintEvent ( QPaintEvent *)
{
    QPainter painter(this);
    painter.setWindow(0, 0, WIDTH, HEIGHT);
    for(int i = 0; i<frame.size(); ++i)
    {
        unsigned char grey = (unsigned char)frame.at(i);
        painter.fillRect(i,0,1,HEIGHT,QColor(grey,grey,grey));
    }
}
