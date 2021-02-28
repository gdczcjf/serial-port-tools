#include "main_window.h"
#include "ui_main_window.h"

#include <QList>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QSettings>
#include <QSerialPortInfo>
#include <QWidget>

#include "about_dialog.h"
#include "com/com_tab.h"
#include "oscilloscope/oscilloscope_tab.h"
#include "linear_ccd/linear_ccd_tab.h"
#include "ccd/ccd_tab.h"
#include "about_dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_com(nullptr),
    m_comTab(new ComTab(0, &m_com)),
    m_oscilloscope(new OscilloscopeTab),
    m_linearCdd(new LinearCcdTab),
    m_ccd(new CcdTab)
{
    ui->setupUi(this);

    this->setWindowTitle("串口调试工具");
    udataComList();

    ui->openPort->setCheckable(true);

    ui->tabWidget->clear();
    QHBoxLayout *layout = new QHBoxLayout;
    ui->tabWidget->setLayout(layout);
    ui->tabWidget->addTab(m_comTab, "串口");
    ui->tabWidget->addTab(m_oscilloscope, "示波器");
    ui->tabWidget->addTab(m_linearCdd, "线性ccd");
    ui->tabWidget->addTab(m_ccd, "摄像头");

    QObject::connect(this, SIGNAL(comOpen()), m_ccd, SLOT(onComOpen()));
    QObject::connect(this, SIGNAL(comClose()), m_ccd, SLOT(onComClose()));

    loadSetting();
}

MainWindow::~MainWindow()
{
    saveSetting();
    closePort();
    delete ui;
    delete m_comTab;
    delete m_oscilloscope;
    delete m_linearCdd;
    delete m_ccd;
}

void MainWindow::on_updataComBnt_clicked()
{
    udataComList();
}


void MainWindow::udataComList()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    ui->comList->clear();
    QString portName;
    for(int i = 0; i < ports.size(); i++)
    {
        portName = ports.at(i).portName();
        if(portName.toLower().startsWith("com"))
            ui->comList->addItem(portName);
    }
}

void MainWindow::on_openPort_clicked()
{
    if(m_com)
        closePort();
    else
        openPort();

    if(m_com)
    {
        ui->openPort->setText("关闭串口");
        ui->comList->setEnabled(false);
        ui->baudrateList->setEnabled(false);
        ui->updataComBnt->setEnabled(false);
        ui->ParityList->setEnabled(false);
        ui->DataBitsList->setEnabled(false);
        ui->StopBitsList->setEnabled(false);
        for(int i = 0; i < ui->tabWidget->count(); ++i)
        {
            if(i!= ui->tabWidget->currentIndex())
                ui->tabWidget->setTabEnabled(i,false);
        }
    }
    else
    {
        ui->openPort->setText("打开串口");
        ui->comList->setEnabled(true);
        ui->baudrateList->setEnabled(true);
        ui->updataComBnt->setEnabled(true);
        ui->ParityList->setEnabled(true);
        ui->DataBitsList->setEnabled(true);
        ui->StopBitsList->setEnabled(true);
        for(int i = 0; i < ui->tabWidget->count(); ++i)
        {
            ui->tabWidget->setTabEnabled(i,true);
        }
    }
}


void MainWindow::setPort()
{
    if(!m_com)
        return;

    //设置波特率
    if(ui->baudrateList->currentText()==tr("256000"))
        m_com->setBaudRate(256000);
    else if(ui->baudrateList->currentText()==tr("128000"))
        m_com->setBaudRate(128000);
    else if(ui->baudrateList->currentText()==tr("115200"))
        m_com->setBaudRate(115200);
    else if(ui->baudrateList->currentText()==tr("57600"))
        m_com->setBaudRate(57600);
    else if(ui->baudrateList->currentText()==tr("56000"))
        m_com->setBaudRate(56000);
    else if(ui->baudrateList->currentText()==tr("38400"))
        m_com->setBaudRate(38400);
    else if(ui->baudrateList->currentText()==tr("19200"))
        m_com->setBaudRate(19200);
    else if(ui->baudrateList->currentText()==tr("14400"))
        m_com->setBaudRate(14400);
    else if(ui->baudrateList->currentText()==tr("9600"))
        m_com->setBaudRate(9600);
    else if(ui->baudrateList->currentText()==tr("4800"))
        m_com->setBaudRate(4800);
    else if(ui->baudrateList->currentText()==tr("2400"))
        m_com->setBaudRate(2400);
    else
        m_com->setBaudRate(9600);

    //todo
    m_com->setDataBits(static_cast<QSerialPort::DataBits>(3-ui->DataBitsList->currentIndex()));
    //todo
    m_com->setParity(static_cast<QSerialPort::Parity>(ui->ParityList->currentIndex()));

    switch(ui->StopBitsList->currentIndex())
    {
    case 0:
        m_com->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        m_com->setStopBits(QSerialPort::TwoStop);
        break;
    }
    //m_com->setStopBits(QSerialPort::OneStop);
    m_com->setFlowControl(QSerialPort::NoFlowControl);
    m_com->waitForBytesWritten(500);
    m_com->waitForReadyRead(500);
}

void MainWindow::openPort()
{
    QString portName = ui->comList->currentText(); //获取串口名

    // 处理串口号大于10不能打开串口的问题
    #ifdef _TTY_WIN_
    bool ok;
    int index = portName.remove(0, 3).toInt(&ok);

    if(!ok)
    {
        QMessageBox::critical(this, "错误", "不能打开指定串口\n请检查串口是否被占用");
        return;
    }
    else
    {
        if(9 < index)
            portName.prepend("\\\\.\\COM");// \\.\com11
        else
            portName.prepend("COM");
    }
    #endif

    //定义串口对象，并传递参数，在构造函数里对其进行初始化
    m_com = new QSerialPort(this); //( QextSerialBase::EventDriven);
    m_com->setPortName(portName);

    //以可读写方式打开串口
    m_com->open(QIODevice::ReadWrite);
    if(!m_com->isOpen())
    {
        QMessageBox::critical(this, "错误", "无法打开该串口！");
        delete m_com;
        m_com = 0;
        return;
    }
    setPort();
    connect(m_com, SIGNAL(readyRead()), this, SLOT(onReadData()));
    emit(comOpen());
}

void MainWindow::closePort()
{
    if(!m_com)
        return;
    m_com->close();
    delete m_com;
    m_com = 0;
    emit(comClose());
}

void MainWindow::onReadData()
{
    if(m_com == 0)
        return;
    QByteArray temp = m_com->readAll();
    m_dataGeted.append(temp);
    switch(ui->tabWidget->currentIndex())
    {
    case 0:
        m_comTab->getData(m_dataGeted);
        break;
    case 1:
        m_oscilloscope->takeData(m_dataGeted);
        break;
    case 2:
        m_linearCdd->takeFrame(m_dataGeted);
        break;
    case 3:
        m_ccd->takeFrame(m_dataGeted);
        break;
    }
}


void MainWindow::onLinkClick(QString href)
{
    QDesktopServices::openUrl(QUrl(href));
}
/*
void MainWindow::on_tabWidget_currentChanged(QWidget *)
{
    if(widget)
    ui->tabWidget->resize(widget->size()+QSize(100,0));
    this->adjustSize();
    this->resize(sizeHint() + QSize(100,0));
}*/

void MainWindow::on_exit_triggered()
{
    this->close();
}

void MainWindow::on_about_triggered()
{
    AboutDialog win;
    win.exec();
}

void MainWindow::saveSetting()
{
    QSettings s;
    s.setValue("MainWindow_Geometry", saveGeometry());
    s.setValue("MainWindow_curTab", ui->tabWidget->currentIndex());
    s.setValue("MainWindow_BaudRate",ui->baudrateList->currentIndex());
    s.setValue("MainWindow_Parity",ui->ParityList->currentIndex());
    s.setValue("MainWindow_DataBits",ui->DataBitsList->currentIndex());
    s.setValue("MainWindow_StopBits", ui->StopBitsList->currentIndex());
    s.setValue("MainWindow_Com", ui->comList->currentIndex());

}

void MainWindow::loadSetting()
{
    QSettings s;
    restoreGeometry(s.value("MainWindow_Geometry").toByteArray());
    ui->tabWidget->setCurrentIndex(s.value("MainWindow_curTab").toInt());
    ui->baudrateList->setCurrentIndex(s.value("MainWindow_BaudRate", 8).toInt());
    ui->ParityList->setCurrentIndex(s.value("MainWindow_Parity").toInt());
    ui->DataBitsList->setCurrentIndex(s.value("MainWindow_DataBits").toInt());
    ui->StopBitsList->setCurrentIndex(s.value("MainWindow_StopBits").toInt());
    ui->comList->setCurrentIndex(s.value("MainWindow_Com").toInt());
}
