#ifndef CCD_TAB_H
#define CCD_TAB_H

#include <QWidget>
#include <QByteArray>

#include "pic_view.h"
#include "utilities.h"

namespace Ui {
class CcdTab;
}

class CcdTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit CcdTab(QWidget *parent = 0);
    ~CcdTab();

    void takeFrame(QByteArray& data);

public slots:
    void onComOpen();
    void onComClose();
    
private slots:
    void on_spinBoxWidth_valueChanged(int arg1);

    void on_spinBoxHeight_valueChanged(int arg1);

    void on_radioButtonAutoZoom_clicked();

    void on_radioButtonNoZoom_clicked();

    void on_spinBoxTh_valueChanged(int arg1);

    void on_pushButtonSaveData_clicked();

    void on_pushButtonSavePicture_clicked();

    void getCursorValue(int x, int y, int value);

    void on_horizontalSliderTh_valueChanged(int value);

    void on_radioButton_fixed_clicked();

    void on_pushButton_openDll_clicked();

    void on_radioButton_custom_clicked();

    void on_radioButton_mid_clicked();

private:
    void init();
    void saveSettings();
    void loadSettings();

    void updateDisplay();
    void updatePicView();
    void updateBinPicView();
    void updateValue();

    void loadDll();

    Ui::CcdTab *ui;
    PicView m_picView;
    PicView m_binPicView;
    QByteArray m_dataFrame;
    int m_th;
    ImgProcFunc m_imgProcFunc;

};

#endif // CCD_TAB_H
