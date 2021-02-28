#ifndef WAVEVIEW_H
#define WAVEVIEW_H

#include <QWidget>
#include <QByteArray>
#include <QPainter>
#include <QMouseEvent>

class WaveView : public QWidget
{
    Q_OBJECT
public:
    explicit WaveView(QWidget *parent = 0);
    void disp(const QByteArray &dat, unsigned char th);

signals:
    
public slots:

protected:
    virtual void paintEvent ( QPaintEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );

private:
    QByteArray frame;
    unsigned char th;
    QPoint transPoint(int x, int y);
    QPoint reTransPoint(int x, int y);
    void drawCoordinate(QPainter &painter);
signals:
    void mouseMove(QPoint pos);
    
};

#endif // WAVEVIEW_H
