#include "com_tab.h"
#include "ui_com_tab.h"

#include <QMessageBox>
#include <QFileDialog>
#include <fstream>
#include <QThread>
#include <QSettings>
#include <QSerialPort>

#include "utilities.h"

ComTab::ComTab(QWidget *parent, QSerialPort **ppCom) :
    QWidget(parent),
    ui(new Ui::ComTab),
    m_ppCom(ppCom)
{
    ui->setupUi(this);
    ui->DataGetted->setReadOnly(true);
    ui->DataGetted->setWordWrapMode(QTextOption::WrapAnywhere);
    ui->DataGetted->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->DataGetted->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->DataSend->setWordWrapMode(QTextOption::WrapAnywhere);
    ui->DataSend->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->DataSend->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    loadSettings();
}

ComTab::~ComTab()
{
    saveSettings();
    delete ui;
}

void ComTab::getData(QByteArray &data)
{
    QString str;
    if(ui->GettedTextBnt->isChecked())
    {
        Utilities::byteToText(data, str);
    }
    else
    {
        Utilities::byteToHexText(data,str,m_dataGetted.size());
    }
    ui->DataGetted->moveCursor(QTextCursor::End);
    ui->DataGetted->insertPlainText(str);
    m_dataGetted.append(data);
    data.clear();
}

void ComTab::cleanTextGetted()
{
    m_dataGetted.clear();
    ui->DataGetted->clear();
}



void ComTab::on_GettedTextBnt_clicked()
{
    ui->DataGetted->clear();
    ui->DataGetted->setPlainText(QString::fromUtf8(m_dataGetted.data()));
}

void ComTab::on_GettedHexBnt_clicked()
{
    ui->DataGetted->clear();
    QString str;
    Utilities::byteToHexText(m_dataGetted,str);
    ui->DataGetted->setPlainText(str);
}

void ComTab::on_CleanDataGettedBnt_clicked()
{
    cleanTextGetted();
}

void ComTab::on_CleanDataSendBnt_clicked()
{
    ui->DataSend->clear();
}

void ComTab::on_SendBnt_clicked()
{
    if((*m_ppCom) == 0 || !(*m_ppCom)->isOpen())
    {
        QMessageBox::information(this, "提示", "请先打开串口。");
        return;
    }
    QByteArray data;
    if(ui->SendHexBnt->isChecked())
    {
        if(!Utilities::hexTextToByte(ui->DataSend->toPlainText(),data))
        {
           QMessageBox::critical(this, "错误", "不是有效的十六进制表示格式！\n"
                                 "请使用0-9和A-F表示，并用空格隔开每个字节。\n"
                                 "如： FA 29 AA BB 3B");
           return;
        }
    }
    else if(ui->SendTextBnt->isChecked())
    {
        data = ui->DataSend->toPlainText().toUtf8();
    }
    if(data.size() > 0 && (*m_ppCom) != 0 && (*m_ppCom)->isOpen())
    {
        if(data.size() > 4096)
        {
            QMessageBox::critical(this,"警告","发送数据大于4096字节，多余部分不能发送！");
        }
        (*m_ppCom)->write(data);

    }
}

void ComTab::on_SaveDataGettedBnt_clicked()
{
    QString fileName;
    if(ui->GettedHexBnt->isChecked())
        fileName = QFileDialog::getSaveFileName(this);
    else if(ui->GettedTextBnt->isChecked())
        fileName = QFileDialog::getSaveFileName(this, "", "", "文本文件(*.txt)");
    if(fileName == "")
        return;
    std::fstream fs(fileName.toUtf8().data(), std::ios_base::out | std::ios_base::binary);
    if(!fs.good())
    {
        QMessageBox::critical(this,"错误",QString("无法打开文件") + fileName + QString("，是否被其他程序占用？"));
        return;
    }
    fs.write(m_dataGetted.data(), m_dataGetted.size());
    fs.close();
}

void ComTab::on_OpenDataBnt_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName == "")
        return;
    std::fstream fs;
    fs.open(fileName.toUtf8().data(), std::ios_base::in | std::ios_base::binary);
    if(!fs.good())
    {
        QMessageBox::critical(this,"错误",QString("无法打开文件") + fileName + QString("，是否被其他程序占用？"));
        return;
    }
    // get length of file:
    fs.seekg (0, fs.end);
    int length = fs.tellg();
    fs.seekg (0, fs.beg);
    QByteArray data;
    data.resize(length);
    fs.read(data.data(), length);

    if(fileName.section('.',-1) == "txt")
    {
        ui->SendHexBnt->setChecked(false);
        ui->SendTextBnt->setChecked(true);
        ui->DataSend->setPlainText(QString::fromUtf8(data));
    }
    else
    {
        ui->SendHexBnt->setChecked(true);
        ui->SendTextBnt->setChecked(false);
        QString str;
        Utilities::byteToHexText(data,str);
        ui->DataSend->setPlainText(str);
    }
}

void ComTab::saveSettings()
{
    QSettings settings;
    settings.setValue("ComTab_SplitterSizes", ui->splitter->saveState());
    settings.setValue("ComTab_TextSend", ui->DataSend->toPlainText());
}

void ComTab::loadSettings()
{
    QSettings settings;
    ui->splitter->restoreState(settings.value("ComTab_SplitterSizes").toByteArray());
    ui->DataSend->setPlainText(settings.value("ComTab_TextSend").toString());
}
