#ifndef PICVIEW_H
#define PICVIEW_H

#include <QWidget>
#include <QImage>
#include <QMouseEvent>

class PicView : public QWidget
{
    Q_OBJECT
public:
    explicit PicView(QWidget *parent = 0);
    ~PicView();
    bool disp(const QByteArray &dat);
    bool setPicSize(int width, int height);
    void setAutoZoom(bool b);
    QImage *getImage();

signals:
    void cursorValue(int x, int y, int value);

public slots:

protected:
    virtual void paintEvent ( QPaintEvent * event );
    void mouseMoveEvent ( QMouseEvent * event );

private:
    void changeDispWay();

    QImage *m_image;
    bool m_isAutoZoom;
};

#endif // PICVIEW_H
