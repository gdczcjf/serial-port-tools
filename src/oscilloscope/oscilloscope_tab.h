#ifndef OSCILLOSCOPE_TAB_H
#define OSCILLOSCOPE_TAB_H

#include <QWidget>
#include <QByteArray>

namespace Ui {
class OscilloscopeTab;
}

class OscilloscopeWave;

class OscilloscopeTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit OscilloscopeTab(QWidget *parent = 0);
    ~OscilloscopeTab();
     void takeData(QByteArray &dat);

private slots:
    void on_pushButton_CH1_clicked();

    void on_dial_yScaleIndex_valueChanged(int value);

    void on_pushButton_CH2_clicked();

    void on_pushButton_CH3_clicked();

    void on_pushButton_CH4_clicked();

    void on_dial_xScaleIndex_valueChanged(int value);

    void on_doubleSpinBox_ch1max_valueChanged(double arg1);

    void on_doubleSpinBox_ch1min_valueChanged(double arg1);

    void on_doubleSpinBox_ch2max_valueChanged(double arg1);

    void on_doubleSpinBox_ch2min_valueChanged(double arg1);

    void on_doubleSpinBox_ch3max_valueChanged(double arg1);

    void on_doubleSpinBox_ch3min_valueChanged(double arg1);

    void on_doubleSpinBox_ch4max_valueChanged(double arg1);

    void on_doubleSpinBox_ch4min_valueChanged(double arg1);

    void on_pushButton_clear_clicked();

private:
    void saveSetting();
    void loadSetting();


    double m_valMin[4];
    double m_valMod[4];

    Ui::OscilloscopeTab *ui;
    OscilloscopeWave *m_wave;
};

#endif // OSCILLOSCOPE_TAB_H
