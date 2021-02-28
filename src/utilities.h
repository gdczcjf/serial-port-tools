#ifndef UTILITIES_H
#define UTILITIES_H

#include <QByteArray>
#include <QChar>
#include <QString>

typedef unsigned int uint32;
static_assert(sizeof(uint32) == 4);
typedef void (*ImgProcFunc)(const char *in, char *out, uint32 width, uint32 height); //Cluster Algorithm Based On Spatial Distribution Analysis

class Utilities
{
public:
    Utilities();

    static void byteToText(const QByteArray &byte, QString &str);
    static void byteToHexText(const QByteArray &byte, QString &str, int pos = 0);//posָ��λ�ã���λ��Ϊ15ʱ�ỻ��
    static bool hexTextToByte(const QString &str, QByteArray &byte);
    static bool isWhiteSpace(QChar ch);
    static QByteArray charToHex(unsigned char ch);

    static void toBinary(const QByteArray &,QByteArray &, unsigned char th);
    static bool doStatistics(const QByteArray &data,
                                 unsigned int &min,
                                 unsigned int &max,
                                 unsigned int &sum);

    static bool loadDll(QString *name, ImgProcFunc *pFun);
};

#endif // UTILITIES_H
