#include "oscilloscope_tab.h"
#include "ui_oscilloscope_tab.h"

#include "oscilloscope_screen.h"

#include <QSettings>
#include <QMessageBox>

OscilloscopeTab::OscilloscopeTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OscilloscopeTab),
    m_wave(new OscilloscopeWave)
{
    ui->setupUi(this);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_wave);
    ui->frame_wave->setLayout(layout);

    on_pushButton_CH1_clicked();

    loadSetting();
}

OscilloscopeTab::~OscilloscopeTab()
{
    saveSetting();
    delete ui;
    delete m_wave;
}

void OscilloscopeTab::takeData(QByteArray &dat)
{
    while(dat.size() >= 10)
    {
        int index = 0;

        while(index < dat.size() - 1)
        {
            if(dat[index] == (char)0xff && dat[index + 1] == (char)0xff)
            {
                index += 2;
                break;
            }
            ++index;
        }
        if(dat.size() - index >= 8)
        {
            for(int i = 0; i < 4; ++i)
            {
                int tmp;
                tmp =  (unsigned char)dat.at(index);
                ++index;
                tmp = (tmp << 8) + (unsigned char)dat.at(index);
                ++index;
                m_wave->appendData(i, ((double)tmp * m_valMod[i] / 65535.0) + m_valMin[i]);
            }
        }
        dat.remove(0,index + 1);
    }
}

void OscilloscopeTab::on_dial_yScaleIndex_valueChanged(int value)
{
    if(m_wave->yScale() != 0)
    {
        m_wave->setYScaleIndex(value);
    }

}

void OscilloscopeTab::on_pushButton_CH1_clicked()
{
    if(m_wave->curCh() != 0)
    {
        m_wave->setCurCh(0);
    }
    else if(m_wave->yScale() != 0)
    {
            m_wave->setYScaleIndex(0);
            return;
    }

    if(m_wave->yScale() == 0)
        m_wave->setYScaleIndex(ui->dial_yScaleIndex->value());
}

void OscilloscopeTab::on_pushButton_CH2_clicked()
{
    if(m_wave->curCh() != 1)
    {
        m_wave->setCurCh(1);
    }
    else if(m_wave->yScale() != 0)
    {
            m_wave->setYScaleIndex(0);
            return;
    }
    if(m_wave->yScale() == 0)
        m_wave->setYScaleIndex(ui->dial_yScaleIndex->value());
}

void OscilloscopeTab::on_pushButton_CH3_clicked()
{
    if(m_wave->curCh() != 2)
    {
        m_wave->setCurCh(2);
    }
    else if(m_wave->yScale() != 0)
    {
            m_wave->setYScaleIndex(0);
            return;
    }
    if(m_wave->yScale() == 0)
        m_wave->setYScaleIndex(ui->dial_yScaleIndex->value());
}

void OscilloscopeTab::on_pushButton_CH4_clicked()
{
    if(m_wave->curCh() != 3)
    {
        m_wave->setCurCh(3);
    }
    else if(m_wave->yScale() != 0)
    {
            m_wave->setYScaleIndex(0);
            return;
    }
    if(m_wave->yScale() == 0)
        m_wave->setYScaleIndex(ui->dial_yScaleIndex->value());
}

void OscilloscopeTab::on_dial_xScaleIndex_valueChanged(int value)
{
    m_wave->setXScaleIndex(value);
}

void OscilloscopeTab::on_doubleSpinBox_ch1max_valueChanged(double arg1)
{
    m_valMod[0] = arg1 - ui->doubleSpinBox_ch1min->value();
    if(m_valMod[0] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch1max->setValue(1.0);
        ui->doubleSpinBox_ch1min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch1min_valueChanged(double arg1)
{
    m_valMin[0] = arg1;
    m_valMod[0] = ui->doubleSpinBox_ch1max->value() - arg1;
    if(m_valMod[0] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch1max->setValue(1.0);
        ui->doubleSpinBox_ch1min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch2max_valueChanged(double arg1)
{
    m_valMod[1] = arg1 - ui->doubleSpinBox_ch2min->value();
    if(m_valMod[1] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch2max->setValue(1.0);
        ui->doubleSpinBox_ch2min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch2min_valueChanged(double arg1)
{
    m_valMin[1] = arg1;
    m_valMod[1] = ui->doubleSpinBox_ch2max->value() - arg1;
    if(m_valMod[1] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch2max->setValue(1.0);
        ui->doubleSpinBox_ch2min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch3max_valueChanged(double arg1)
{
    m_valMod[2] = arg1 - ui->doubleSpinBox_ch3min->value();
    if(m_valMod[2] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch3max->setValue(1.0);
        ui->doubleSpinBox_ch3min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch3min_valueChanged(double arg1)
{
    m_valMin[2] = arg1;
    m_valMod[2] = ui->doubleSpinBox_ch3max->value() - arg1;
    if(m_valMod[2] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch3max->setValue(1.0);
        ui->doubleSpinBox_ch3min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch4max_valueChanged(double arg1)
{
    m_valMod[3] = arg1 - ui->doubleSpinBox_ch4min->value();
    if(m_valMod[3] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch4max->setValue(1.0);
        ui->doubleSpinBox_ch4min->setValue(0.0);
    }
}

void OscilloscopeTab::on_doubleSpinBox_ch4min_valueChanged(double arg1)
{
    m_valMin[3] = arg1;
    m_valMod[3] = ui->doubleSpinBox_ch4max->value() - arg1;
    if(m_valMod[3] <= 0)
    {
        QMessageBox::critical(this, "错误","最大值小于或等于最小值");
        ui->doubleSpinBox_ch4max->setValue(1.0);
        ui->doubleSpinBox_ch4min->setValue(0.0);
    }
}

void OscilloscopeTab::on_pushButton_clear_clicked()
{
    m_wave->clearData();
}

void OscilloscopeTab::saveSetting()
{
    QSettings s;
    s.setValue("Oscilloscope_Ch1Max", ui->doubleSpinBox_ch1max->value());
    s.setValue("Oscilloscope_Ch2Max", ui->doubleSpinBox_ch2max->value());
    s.setValue("Oscilloscope_Ch3Max", ui->doubleSpinBox_ch3max->value());
    s.setValue("Oscilloscope_Ch4Max", ui->doubleSpinBox_ch4max->value());

    s.setValue("Oscilloscope_Ch1Min", ui->doubleSpinBox_ch1min->value());
    s.setValue("Oscilloscope_Ch2Min", ui->doubleSpinBox_ch2min->value());
    s.setValue("Oscilloscope_Ch3Min", ui->doubleSpinBox_ch3min->value());
    s.setValue("Oscilloscope_Ch4Min", ui->doubleSpinBox_ch4min->value());

    s.setValue("Oscilloscope_xScaleIndex", ui->dial_xScaleIndex->value());
    s.setValue("Oscilloscope_YScaleIndex", ui->dial_yScaleIndex->value());
}

void OscilloscopeTab::loadSetting()
{
    QSettings s;

    ui->doubleSpinBox_ch1max->setValue(s.value("Oscilloscope_Ch1Max", 5.0).toDouble());
    ui->doubleSpinBox_ch2max->setValue(s.value("Oscilloscope_Ch2Max", 5.0).toDouble());
    ui->doubleSpinBox_ch3max->setValue(s.value("Oscilloscope_Ch3Max", 5.0).toDouble());
    ui->doubleSpinBox_ch4max->setValue(s.value("Oscilloscope_Ch4Max", 5.0).toDouble());

    ui->doubleSpinBox_ch1min->setValue(s.value("Oscilloscope_Ch1Min", 0.0).toDouble());
    ui->doubleSpinBox_ch2min->setValue(s.value("Oscilloscope_Ch2Min", 0.0).toDouble());
    ui->doubleSpinBox_ch3min->setValue(s.value("Oscilloscope_Ch3Min", 0.0).toDouble());
    ui->doubleSpinBox_ch4min->setValue(s.value("Oscilloscope_Ch4Min", 0.0).toDouble());

    m_valMin[0] = ui->doubleSpinBox_ch1min->value();
    m_valMin[1] = ui->doubleSpinBox_ch2min->value();
    m_valMin[2] = ui->doubleSpinBox_ch3min->value();
    m_valMin[3] = ui->doubleSpinBox_ch4min->value();

    m_valMod[0] = ui->doubleSpinBox_ch1max->value() - m_valMin[0];
    m_valMod[1] = ui->doubleSpinBox_ch2max->value() - m_valMin[1];
    m_valMod[2] = ui->doubleSpinBox_ch3max->value() - m_valMin[2];
    m_valMod[3] = ui->doubleSpinBox_ch4max->value() - m_valMin[3];

    ui->dial_xScaleIndex->setValue(s.value("Oscilloscope_xScaleIndex", 5).toInt());
    ui->dial_yScaleIndex->setValue(s.value("Oscilloscope_yScaleIndex", 5).toInt());

    on_dial_xScaleIndex_valueChanged(ui->dial_xScaleIndex->value());
    on_dial_yScaleIndex_valueChanged(ui->dial_yScaleIndex->value());

}
