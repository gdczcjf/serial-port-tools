#include "linear_ccd_tab.h"
#include "ui_linear_ccd_tab.h"
#include <QList>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include "utilities.h"

LinearCcdTab::LinearCcdTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LinearCcdTab),
    m_imgProcFunc(0)
{
    //ui
    ui->setupUi(this);
    QHBoxLayout *layout3 = new QHBoxLayout;
    layout3->addWidget(&m_greyView);
    ui->groupBox_3->setLayout(layout3);

    QHBoxLayout *layout4 = new QHBoxLayout;
    layout4->addWidget(&m_binView);
    ui->groupBox_4->setLayout(layout4);


    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget(&m_waveView);
    ui->groupBox_2->setLayout(layout2);

    ui->spinBoxTh->setMaximum(255);
    ui->spinBoxTh->setMinimum(0);

    ui->horizontalSliderTh->setMaximum(255);
    ui->horizontalSliderTh->setMinimum(0);


    //data
    m_dataFrame.fill(0,128);

    //signal
    QObject::connect(&m_waveView,SIGNAL(mouseMove(QPoint)),this,SLOT(onMouseMove(QPoint)));

}

LinearCcdTab::~LinearCcdTab()
{
    delete ui;
}

void LinearCcdTab::takeFrame(QByteArray& data)
{
    if(data.size() > 128)
    {
        int index = data.lastIndexOf(0xff);
        if(index == -1)
            return;

        if(index >= 128)
            m_dataFrame = data.mid(index-128,128);

        data.remove(0,index + 1);
        procFrame();
    }
}


unsigned int LinearCcdTab::doStatistics(const QByteArray &data)
{
    unsigned int max,min,mid,sum;
    max = min = sum = (unsigned char)data.at(0);
    for(int i = 1; i < data.size(); ++i)
    {
       if((unsigned char)data.at(i) > max) max = (unsigned char)data.at(i);
       if((unsigned char)data.at(i) < min) min = (unsigned char)data.at(i);
       sum += (unsigned char)data.at(i);
    }
    mid = (max+min)/2;
    ui->maxText->setText(QString::number(max));
    ui->minText->setText(QString::number(min));
    ui->aveText->setText(QString::number(sum/data.size()));
    ui->midText->setText(QString::number(mid));
    return mid;
}

void LinearCcdTab::procFrame()
{
    if( m_dataFrame.size() != 128 )
        return;
    QByteArray binData;
    unsigned int t = doStatistics(m_dataFrame);
    m_greyView.disp(m_dataFrame);

    unsigned char th=0;
    if(ui->radioButton_midVal->isChecked())
    {
        th = t;
        Utilities::toBinary(m_dataFrame,binData,th);
    }
    else if(ui->radioButton_fixedVal->isChecked())
    {
        th = ui->spinBoxTh->value();
        Utilities::toBinary(m_dataFrame,binData,th);
    }
    else if(ui->radioButton_custom->isChecked())
    {
        binData.resize(m_dataFrame.size());
        if(m_imgProcFunc != 0)
        {
            m_imgProcFunc(m_dataFrame.constData(), binData.data(), m_dataFrame.size(), 1);
        }
        else
        {
            binData.fill(0);
        }
    }

    m_waveView.disp(m_dataFrame,th);
    m_binView.disp(binData);
}

void LinearCcdTab::on_spinBoxTh_valueChanged(int value)
{
    ui->horizontalSliderTh->setValue(value);
    if(ui->radioButton_fixedVal->isChecked())
        procFrame();
}

void LinearCcdTab::onMouseMove(QPoint pos)
{
    pos.setX(((pos.x() >= 0 && pos.x() < 128)? pos.x() : 0));
    pos.setY(((pos.y() >= 0 && pos.y() < 256)? pos.y() : 0));
    ui->pos->setText(QString::number(pos.x()) + "," + QString::number(pos.y()));
    ui->val->setText(QString::number((unsigned char)(m_dataFrame.at(pos.x()))));
}


void LinearCcdTab::on_horizontalSliderTh_valueChanged(int value)
{
    ui->spinBoxTh->setValue(value);
}

void LinearCcdTab::on_radioButton_midVal_clicked()
{
    procFrame();
}

void LinearCcdTab::on_radioButton_fixedVal_clicked()
{
    procFrame();
}

void LinearCcdTab::loadDll()
{
    QString name;
    Utilities::loadDll(&name, &m_imgProcFunc);
    if(name != "")
        ui->lineEdit_dllPath->setText(name);
}

void LinearCcdTab::on_radioButton_custom_clicked()
{
    if(m_imgProcFunc == 0)
        loadDll();
    procFrame();
}


void LinearCcdTab::on_pushButton_openDll_clicked()
{
    loadDll();
}
