#ifndef OSCILLOSCOPEWAVE_H
#define OSCILLOSCOPEWAVE_H

#include <QWidget>
#include <QList>
#include <QColor>
#include <QTimer>

class QPixmap;

class OscilloscopeWave : public QWidget
{
    Q_OBJECT
public:
    explicit OscilloscopeWave(QWidget *parent = 0);
    ~OscilloscopeWave();

    virtual void paintEvent(QPaintEvent * event);

    void clearData();
    void appendData(int ch, double val);
    void setXScaleIndex(size_t xScaleIndex);
    void setYScaleIndex(size_t yScaleIndex);
    double yScale();
    void setCurCh(size_t ch);
    size_t curCh();

signals:
    
public slots:

private:
    static const char *yScaleTextFromIndex(size_t i);
    static int xScaleFromIndex(size_t i);
    static double yScaleFromIndex(size_t i);

    void drawGrid(QPainter &painter);
    void drawWave(QPainter &painter);
    void drawWave(QPainter &painter, QList<double> &dat, double yScale);

    void displayScale(QPainter &painter, int x, int y);

    int m_xMaxVal;
    double m_yMaxVal;

    QList<double> m_data[4];
    double m_xScale;
    double m_yScale[4];
    QString m_yScaleText[4];
    size_t m_curCh;
    QTimer m_timer;


    const int BORDER_SIZE_TOP = 10;
    const int BORDER_SIZE_BOTTOM = 30;
    const int BORDER_SIZE_RIGHT = 10;
    const int BORDER_SIZE_LEFT = 10;
    static const QColor m_chColor[4];
    
};

#endif // OSCILLOSCOPEWAVE_H
