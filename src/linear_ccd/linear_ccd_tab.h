#ifndef LINEAR_CCD_TAB_H
#define LINEAR_CCD_TAB_H

#include <QWidget>
#include <QPoint>
#include <QPixmap>
#include <QLabel>

#include "line_view.h"
#include "wave_view.h"

#include "utilities.h"

namespace Ui {
class LinearCcdTab;
}

class LinearCcdTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit LinearCcdTab(QWidget *parent = 0);
    ~LinearCcdTab();
    void takeFrame(QByteArray& data);

private slots:

    void on_spinBoxTh_valueChanged(int arg1);

    void onMouseMove(QPoint);

    void on_horizontalSliderTh_valueChanged(int value);

    void on_radioButton_midVal_clicked();

    void on_radioButton_fixedVal_clicked();

    void on_radioButton_custom_clicked();

    void on_pushButton_openDll_clicked();

private:
    Ui::LinearCcdTab *ui;


    unsigned int doStatistics(const QByteArray &);
    void procFrame();
    void loadDll();

    GreyView m_greyView;
    WaveView m_waveView;
    GreyView m_binView;
    QByteArray m_dataFrame;
    ImgProcFunc m_imgProcFunc;
};

#endif // LINEAR_CCD_TAB_H
