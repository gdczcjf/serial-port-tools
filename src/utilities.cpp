#include "utilities.h"

#include <QLibrary>
#include <QMessageBox>
#include <QFileDialog>

Utilities::Utilities()
{
}

void Utilities::byteToHexText(const QByteArray &byte, QString &str, int pos)
{
    str.clear();
    for(int i = 0; i < byte.size(); ++i)
    {
        int tmp = (pos + i) % 16;
        str.append(charToHex(byte.at(i)));
        if(tmp == 15)
            str.append('\n');
        else
            str.append(' ');
    }
}

void Utilities::byteToText(const QByteArray &byte, QString &str)
{
    str=QString::fromUtf8(byte.data());
}

bool Utilities::hexTextToByte(const QString &str, QByteArray &byte)
{
    QString tmp;
    QString::ConstIterator it = str.begin();
    for(; it != str.end(); ++it)
    {
        if(!isWhiteSpace(*it))
        {
            tmp.append(*it);
        }
        else
        {
            if(tmp.size() == 2 || tmp.size() ==1)
            {
               bool ok;
               byte.append(tmp.toInt(&ok,16));
               tmp.clear();
               if(!ok)
               {
                   byte.clear();
                   return false;
               }
            }
            else if(tmp.size() > 2)
            {
                byte.clear();
                return false;
            }
        }
    }
    if(tmp.size() == 2 || tmp.size() ==1)
    {
       bool ok;
       byte.append(tmp.toInt(&ok,16));
       tmp.clear();
       if(!ok)
       {
           byte.clear();
           return false;
       }
    }
    else if(tmp.size() > 2)
    {
        byte.clear();
        return false;
    }
    return true;
}

bool Utilities::isWhiteSpace(QChar ch)
{
    return ch == QChar(' ')
            || ch == QChar('\t')
            || ch == QChar('\n')
            || ch == QChar('\r');
}

QByteArray Utilities::charToHex(unsigned char ch)
{
    static const char arr[16] =
    {
        '0','1','2','3',
        '4','5','6','7',
        '8','9','A','B',
        'C','D','E','F'
    };
    QByteArray tmp;
    tmp.append(arr[ch/16]);
    tmp.append(arr[ch%16]);
    return tmp;
}

void Utilities::toBinary(const QByteArray &data,QByteArray &binData, unsigned char th)
{
    for(int i = 0; i < data.size(); ++i)
    {
        binData.append( (unsigned char)data.at(i) > th? 255 : 0);
    }
}

bool Utilities::doStatistics(const QByteArray &data,
                             unsigned int &min,
                             unsigned int &max,
                             unsigned int &sum)
{
    if(data.size() == 0)
        return false;

    max = min = sum = (unsigned char)data.at(0);
    for(int i = 1; i < data.size(); ++i)
    {
       if((unsigned char)data.at(i) > max) max = (unsigned char)data.at(i);
       if((unsigned char)data.at(i) < min) min = (unsigned char)data.at(i);
       sum += (unsigned char)data.at(i);
    }
    return true;
}

bool Utilities::loadDll(QString *name, ImgProcFunc *pFun)
{
    QString fileName = QFileDialog::getOpenFileName(0, "", "", "动态链接库(*.dll *.so)");
    if(fileName == "")
        return false;

    if(QLibrary::isLibrary(fileName) == false)
    {
        QMessageBox::critical(0,"错误", fileName + QString(" 不是动态链接库文件"));
        return false;
    }

    QLibrary lib(fileName);
    if(lib.load() == false)
    {
        QMessageBox::critical(0,"错误",QString("无法载入") + fileName);
        return false;
    }

    *pFun = (ImgProcFunc) lib.resolve("ImgProc");
    if(*pFun == NULL)
    {
        QMessageBox::critical(0,"错误",QString("无效的动态链接库:") + fileName);
        *name = "无效的动态链接库";
        return false;
    }
    *name = lib.fileName();
    return true;
}
