#include "pic_view.h"

#include <QPainter>
#include <QRect>

PicView::PicView(QWidget *parent) :
    QWidget(parent),
    m_image(0),
    m_isAutoZoom(false)
{
    this->setMouseTracking(true);
    this->setCursor(Qt::CrossCursor);
}

PicView::~PicView()
{
    delete m_image;
}

bool PicView::disp(const QByteArray &dat)
{
    if(m_image == 0 || dat.size() != m_image->width() * m_image->height())
        return false;

    for(int i = 0; i < dat.size(); ++i)
    {
        m_image->setPixel(i % m_image->width(),
                          i / m_image->width(),
                          (unsigned char) dat.at(i));
    }
    this->repaint();
    return true;
}

bool PicView::setPicSize(int width, int height)
{
    if(width <= 0 || height <= 0)
        return false;
    delete m_image;
    m_image = new QImage(width, height, QImage::Format_Indexed8);
    m_image->setColorCount(256);
    for(int i = 0; i<256; ++i)
    {
        m_image->setColor(i, i * (1 + 256 + 256 * 256) + (0xff << 24));
    }

    m_image->fill(60);
    changeDispWay();
    repaint();
    return true;
}

void PicView::setAutoZoom(bool b)
{
    m_isAutoZoom = b;
    changeDispWay();
    repaint();
}

QImage *PicView::getImage()
{
    return m_image;
}

void PicView::paintEvent ( QPaintEvent *)
{
    if(m_image == 0)
        return;
    QPainter painter(this);
    if(m_isAutoZoom)
    {
        QRect rect(0,0,width(),height());
        painter.drawImage(rect, *m_image);
    }
    else
    {
        painter.drawImage(0, 0, *m_image);
    }
}

void PicView::mouseMoveEvent ( QMouseEvent *event)
{
    int x, y;
    if(m_isAutoZoom)
    {
        x = ((double)event->x()) / ((double)width()  / m_image->width());
        y = ((double)event->y()) / ((double)height() / m_image->height());
    }
    else
    {
        x = event->x();
        y = event->y();
    }
    if(x < 0 || x>= m_image->width() || y < 0 || y >= m_image->height())
        return;
    int value = m_image->pixelIndex(x, y);
    emit(cursorValue(x, y, value));
}

void PicView::changeDispWay()
{
    if(m_isAutoZoom)
    {
        this->setMinimumSize(320,240);
        this->setMaximumSize(16777215, 16777215);
    }
    else
    {
        this->setFixedSize(m_image->width(), m_image->height());
    }
}
