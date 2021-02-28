#ifndef COM_ASSISTANT_H
#define COM_ASSISTANT_H

#include <QWidget>
#include <QByteArray>
#include <QString>


namespace Ui {
class ComTab;
}

class QSerialPort;

class ComTab : public QWidget
{
    Q_OBJECT
    
public:
    explicit ComTab(QWidget *parent, QSerialPort **ppCom);
    ~ComTab();
    void getData(QByteArray &data);
    
private slots:
    void on_GettedTextBnt_clicked();

    void on_GettedHexBnt_clicked();

    void on_CleanDataGettedBnt_clicked();

    void on_CleanDataSendBnt_clicked();

    void on_SendBnt_clicked();

    void on_SaveDataGettedBnt_clicked();

    void on_OpenDataBnt_clicked();

private:
    void cleanTextGetted();
    void saveSettings();
    void loadSettings();

    Ui::ComTab *ui;
    QByteArray m_dataGetted;
    QSerialPort **m_ppCom;

};

#endif // COM_ASSISTANT_H
