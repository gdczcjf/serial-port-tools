#ifndef GREYVIEW_H
#define GREYVIEW_H

#include <QWidget>
#include <QByteArray>

class GreyView : public QWidget
{
    Q_OBJECT
public:
    explicit GreyView(QWidget *parent = 0);
    void disp(const QByteArray &dat);
    
signals:
    
public slots:

protected:
    virtual void paintEvent ( QPaintEvent * event );
    
private:
    QByteArray frame;
};

#endif // GREYVIEW_H
