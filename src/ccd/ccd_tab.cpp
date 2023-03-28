#include "ccd_tab.h"
#include "ui_ccd_tab.h"

#include <QSettings>
#include <QDebug>
#include <QByteArray>
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QHBoxLayout>

#include "utilities.h"


CcdTab::CcdTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CcdTab),
    m_imgProcFunc(0)
{
    ui->setupUi(this);
    init();

    QObject::connect(&m_picView, SIGNAL(cursorValue(int,int,int)), this, SLOT(getCursorValue(int,int,int)));
    QObject::connect(&m_binPicView, SIGNAL(cursorValue(int,int,int)), this, SLOT(getCursorValue(int,int,int)));
}

CcdTab::~CcdTab()
{
    saveSettings();
    delete ui;
}


void CcdTab::init()
{
    ui->spinBoxWidth->setMaximum(320);
    ui->spinBoxWidth->setMinimum(1);
    ui->spinBoxHeight->setMaximum(240);
    ui->spinBoxHeight->setMinimum(1);
    ui->spinBoxTh->setMaximum(255);
    ui->spinBoxTh->setMinimum(0);
    ui->horizontalSliderTh->setMaximum(255);
    ui->horizontalSliderTh->setMinimum(0);
    ui->lineEditEndCode->setEnabled(false);

    loadSettings();

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(&m_picView);
    ui->groupBoxPic->setLayout(layout1);
    m_picView.setAutoZoom(ui->radioButtonAutoZoom->isChecked());

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(&m_binPicView);
    ui->groupBoxBinPic->setLayout(layout2);
    m_binPicView.setAutoZoom(ui->radioButtonAutoZoom->isChecked());
}

void CcdTab::saveSettings()
{
    QSettings s;
    s.setValue("CCD_ImgWidth", ui->spinBoxWidth->value());
    s.setValue("CCD_ImgHeight", ui->spinBoxHeight->value());

    s.setValue("CCD_AutoZoom", ui->radioButtonAutoZoom->isChecked());
}

void CcdTab::loadSettings()
{
    QSettings s;

    ui->spinBoxWidth->setValue( s.value("CCD_ImgWidth", 120).toInt());
    ui->spinBoxHeight->setValue( s.value("CCD_ImgHeight", 40).toInt());

    if(s.value("CCD_AutoZoom", true).toBool())
    {
        ui->radioButtonAutoZoom->setChecked(true);
        ui->radioButtonNoZoom->setChecked(false);
    }
    else
    {
        ui->radioButtonAutoZoom->setChecked(false);
        ui->radioButtonNoZoom->setChecked(true);
    }

    ui->lineEditEndCode->setText("ff");
}

void CcdTab::on_spinBoxWidth_valueChanged(int)
{
    m_picView.setPicSize(ui->spinBoxWidth->value(), ui->spinBoxHeight->value());
    m_binPicView.setPicSize(ui->spinBoxWidth->value(), ui->spinBoxHeight->value());
}

void CcdTab::on_spinBoxHeight_valueChanged(int)
{
    m_picView.setPicSize(ui->spinBoxWidth->value(), ui->spinBoxHeight->value());
    m_binPicView.setPicSize(ui->spinBoxWidth->value(), ui->spinBoxHeight->value());
}

void CcdTab::on_radioButtonAutoZoom_clicked()
{
    m_picView.setAutoZoom(ui->radioButtonAutoZoom->isChecked());
    m_binPicView.setAutoZoom(ui->radioButtonAutoZoom->isChecked());
}

void CcdTab::on_radioButtonNoZoom_clicked()
{
    m_picView.setAutoZoom(ui->radioButtonAutoZoom->isChecked());
    m_binPicView.setAutoZoom(ui->radioButtonAutoZoom->isChecked());
}

void CcdTab::takeFrame(QByteArray& data)
{
    int frameSize = ui->spinBoxHeight->value() * ui->spinBoxWidth->value();
    if(data.size() > frameSize)
    {
        int index = data.lastIndexOf(static_cast<char>(0xff));
        if(index == -1)
            return;

        if(index >= frameSize)
            m_dataFrame = data.mid(index - frameSize, frameSize);

        data.remove(0,index + 1);

        updateDisplay();

    }
}

void CcdTab::updateDisplay()
{
    updateValue();
    updatePicView();
    updateBinPicView();
}

void CcdTab::updatePicView()
{
    m_picView.disp(m_dataFrame);
}

void CcdTab::updateBinPicView()
{
    QByteArray arr;
    if(ui->radioButton_custom->isChecked())
    {
        if(m_imgProcFunc)
        {
            if(m_dataFrame.size() == ui->spinBoxWidth->value() * ui->spinBoxHeight->value())
            {
                arr.resize(m_dataFrame.size());
                m_imgProcFunc(m_dataFrame.data(), arr.data(), ui->spinBoxWidth->value(), ui->spinBoxHeight->value());
            }
        }
    }
    else
    {
        Utilities::toBinary(m_dataFrame, arr, m_th);
    }
    m_binPicView.disp(arr);
}

void CcdTab::updateValue()
{
    unsigned int min,max,sum;
    Utilities::doStatistics(m_dataFrame, min, max, sum);
    ui->labelMinValue->setText(QString::number(min));
    ui->labelMaxValue->setText(QString::number(max));
    ui->labelMidValue->setText(QString::number((min + max) / 2));
    if( m_dataFrame.size() != 0)
        ui->labelAverage->setText(QString::number(sum / m_dataFrame.size()));
    if(ui->radioButton_mid->isChecked())
        m_th = (min + max) / 2;
}

void CcdTab::on_radioButton_fixed_clicked()
{
    m_th = ui->spinBoxTh->value();
    updateBinPicView();
}


void CcdTab::on_spinBoxTh_valueChanged(int arg1)
{
    ui->horizontalSliderTh->setValue(arg1);
    if(ui->radioButton_fixed->isChecked())
    {
        m_th = arg1;
        updateBinPicView();
    }
}

void CcdTab::onComOpen()
{
    ui->spinBoxHeight->setEnabled(false);
    ui->spinBoxWidth->setEnabled(false);
}

void CcdTab::onComClose()
{
    ui->spinBoxHeight->setEnabled(true);
    ui->spinBoxWidth->setEnabled(true);
}

void CcdTab::on_pushButtonSaveData_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "", "", "文本文件(*.txt)");
    if(fileName == "")
        return;
    std::fstream fs(fileName.toUtf8().data(), std::ios_base::out | std::ios_base::binary);
    if(!fs.good())
    {
        QMessageBox::critical(this,"错误",QString("无法打开文件") + fileName + QString("，是否被其他程序占用？"));
        return;
    }
    QString str;
    Utilities::byteToHexText(m_dataFrame, str);
    fs.write(str.toUtf8().data(), str.toUtf8().size());
    fs.close();
}



void CcdTab::on_pushButtonSavePicture_clicked()
{
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "", "", "位图(*.bmp)");
    if(fileName == "")
        return;
    QImage *p = m_picView.getImage();
    if(p == 0)
    {
        QMessageBox::critical(this,"错误",QString("图像为空"));
        return;
    }
    if(!p->save(fileName,"BMP"))
    {
        QMessageBox::critical(this,"错误",QString("无法打开文件") + fileName + QString("，是否被其他程序占用？"));
    }

}

void CcdTab::getCursorValue(int x, int y, int value)
{
    QString str = QString("(%1,%2)").arg(x).arg(y);
    ui->labelCursorPos->setText(str);
    ui->labelPixelValue->setText(QString::number(value));
}

void CcdTab::on_horizontalSliderTh_valueChanged(int value)
{
    ui->spinBoxTh->setValue(value);
}

void CcdTab::on_pushButton_openDll_clicked()
{
    loadDll();
    updateBinPicView();
}

void CcdTab::loadDll()
{
    QString name;
    Utilities::loadDll(&name, &m_imgProcFunc);
    if(name != "")
        ui->lineEdit_dllName->setText(name);
}

void CcdTab::on_radioButton_custom_clicked()
{
    if(m_imgProcFunc ==0)
        loadDll();
    updateBinPicView();
}

void CcdTab::on_radioButton_mid_clicked()
{
    updateDisplay();
}
