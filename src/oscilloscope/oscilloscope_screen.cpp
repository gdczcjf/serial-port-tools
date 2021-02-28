#include "oscilloscope_screen.h"

#include <assert.h>
#include <QPainter>

const QColor OscilloscopeWave::m_chColor[4] =
{
    Qt::yellow, Qt::cyan,Qt::blue,Qt::red
};

OscilloscopeWave::OscilloscopeWave(QWidget *parent) :
    QWidget(parent),
    m_xScale(500.0),
    m_curCh(0)
{
    this->setCursor(Qt::CrossCursor);
    m_yScale[0] = 0; m_yScaleText[0] = "关";
    m_yScale[1] = 0; m_yScaleText[1] = "关";
    m_yScale[2] = 0; m_yScaleText[2] = "关";
    m_yScale[3] = 0; m_yScaleText[3] = "关";
    m_timer.start(100);
    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(repaint()));
}


OscilloscopeWave::~OscilloscopeWave()
{

}

void OscilloscopeWave::clearData()
{
    m_data[0].clear();
    m_data[1].clear();
    m_data[2].clear();
    m_data[3].clear();
}

void OscilloscopeWave::appendData(int ch, double val)
{
    assert(ch >= 0 && ch <4);
    m_data[ch].append(val);
    if(m_data[ch].size() > m_xScale * 10 + 1)
    {
        m_data[ch].pop_front();
    }
}

void OscilloscopeWave::setXScaleIndex(size_t xScaleIndex)
{
    m_xScale = xScaleFromIndex(xScaleIndex);
    repaint();
}

void OscilloscopeWave::setYScaleIndex(size_t yScaleIndex)
{
    assert(m_curCh < 4);
    m_yScale[m_curCh] = yScaleFromIndex(yScaleIndex);
    m_yScaleText[m_curCh] = yScaleTextFromIndex(yScaleIndex);
    repaint();
}

double OscilloscopeWave::yScale()
{
    return m_yScale[m_curCh];
}

void OscilloscopeWave::setCurCh(size_t ch)
{
    assert(ch < 4);
    m_curCh = ch;
    repaint();
}

size_t OscilloscopeWave::curCh()
{
    return m_curCh;
}

const char *OscilloscopeWave::yScaleTextFromIndex(size_t i)
{
    const char* yScaleText[]=
    {
        "关",
        "20V",      "10V",      "5V",
        "2V",       "1V",       "500mV",
        "200mV",    "100mV",    "50mV",
        "20mV",     "10mV",     "5mV",
        "2mV",      "1mV",
    };
    return yScaleText[i];
}

double OscilloscopeWave::yScaleFromIndex(size_t i)
{
    const double yScaleList[15] =
    {
      0,
      20.00,    10.00,  5.000,
      2.000,    1.000,  0.500,
      0.200,    0.100,  0.050,
      0.020,    0.010,  0.005,
      0.002,    0.001
    };
    assert(i < 15);
    return yScaleList[i];
}

int OscilloscopeWave::xScaleFromIndex(size_t i)
{
    const int xScaleList[14] =
    {
        20000,  10000,    5000,
        2000,   1000,     500,
        200,    100,      50,
        20,     10,       5,
        2,      1,
    };
    assert(i < 14);
    return xScaleList[i];
}

void OscilloscopeWave::drawGrid(QPainter &painter)
{
    painter.setPen(QColor(Qt::darkGreen));
    int width = this->width() - BORDER_SIZE_RIGHT - BORDER_SIZE_LEFT;
    int height = this->height() - BORDER_SIZE_TOP - BORDER_SIZE_BOTTOM;
    for(int x = 0; x <= 10; ++x)
    {
        int xPos = x*width/10;
        painter.drawLine(xPos, 0, xPos, -height);
    }
    for(int y = 0; y <= 10; ++y)
    {
        int yPos = - y*height/10;
        painter.drawLine(0, yPos, width, yPos);
    }

    int yMid = height/2;
    int xMid = width/2;
    painter.drawText(-10, - yMid - 5, 10, 10, Qt::AlignCenter, "0");
    for(int i = 0; i <= 50; ++i)
    {
        if(i % 5 != 0)
        {
            int xPos = i * width / 50;
            painter.drawLine(xPos, - yMid -2, xPos, - yMid + 2);
            painter.drawLine(xPos, - height , xPos, - height + 2);
            painter.drawLine(xPos, - 2 , xPos, 0);

            int yPos = i * height / 50;
            painter.drawLine(xMid -2, -yPos, xMid + 2, -yPos);
            painter.drawLine(width, -yPos, width - 2, -yPos);
            painter.drawLine(0, -yPos, 2, -yPos);
        }
    }




}

void OscilloscopeWave::drawWave(QPainter &painter)
{
    for(int i = 0; i < 4; ++i)
    {
        if(m_yScale[i] != 0)
        {
            painter.setPen(m_chColor[i]);
            drawWave(painter, m_data[i], m_yScale[i]);
        }
    }
}

void OscilloscopeWave::drawWave(QPainter &painter, QList<double> &dat, double scale)
{
    if(dat.size() < 2)
        return;
    QList<double>::iterator it = dat.begin();
    int y1 = (*it) / ( scale * 5.0) * (height() - BORDER_SIZE_BOTTOM - BORDER_SIZE_TOP) / 2.0;
    int x1 = 0;
    ++it;
    for(int i = 1; it != dat.end(); ++it, ++i)
    {
        int y2 = (*it) / ( scale * 5.0) * (height() - BORDER_SIZE_BOTTOM - BORDER_SIZE_TOP) / 2.0;
        int x2 = (double)i / (m_xScale * 10.0) * (double)(width() - BORDER_SIZE_RIGHT - BORDER_SIZE_LEFT);
        if(x1 != x2)
        {
            painter.drawLine(x1,-y1,x2,-y1);
            x1 = x2;
        }
        if(y1 != y2)
        {
            painter.drawLine(x2,-y1,x2,-y2);
            y1 = y2;
        }


    }
}

void OscilloscopeWave::displayScale(QPainter &painter, int x, int y)
{
    for(size_t i = 0; i < 4; ++i)
    {
        int xPos = x  + 100 * i;
        if(i == m_curCh)
        {
            painter.setPen(Qt::black);
            painter.fillRect(xPos ,y,45,15, m_chColor[i]);
        }
        else
        {
            painter.setPen(m_chColor[i]);
        }
        painter.drawText(xPos ,y,45,15, Qt::AlignCenter,QString("CH%1").arg(i + 1));
        painter.setPen(m_chColor[i]);
        painter.drawText(xPos + 47,y,45,15, Qt::AlignCenter,m_yScaleText[i]);
        painter.setPen(Qt::white);
        painter.drawRect(xPos + 47,y,44,14);
    }

    painter.setPen(Qt::white);
    painter.drawText(x + 400,y,45,15, Qt::AlignCenter,QString("M"));
    painter.drawText(x + 447,y,45,15, Qt::AlignCenter,QString::number(m_xScale));
    painter.drawRect(x + 447,y,44,14);
}

void OscilloscopeWave::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(0,0,width(),height(),QColor(0,0,0));
    displayScale(painter,BORDER_SIZE_LEFT,height()-20);

    painter.setWindow(-BORDER_SIZE_LEFT, - (height()- BORDER_SIZE_BOTTOM), width(), height());
    drawGrid(painter);
    painter.setWindow(-BORDER_SIZE_LEFT, - ((height() - BORDER_SIZE_TOP - BORDER_SIZE_BOTTOM) / 2 + BORDER_SIZE_TOP), width(), height());
    drawWave(painter);
}
