#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QByteArray>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class ComTab;
class OscilloscopeTab;
class LinearCcdTab;
class CcdTab;
class QSerialPort;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_updataComBnt_clicked();
    void on_openPort_clicked();

    void onReadData();
    void onLinkClick(QString href);

    //void on_tabWidget_currentChanged(QWidget *arg1);

    void on_exit_triggered();

    void on_about_triggered();

signals:
    void comOpen();
    void comClose();

private:
    Ui::MainWindow *ui;
    void udataComList();
    void setPort();
    void openPort();
    void closePort();
    void saveSetting();
    void loadSetting();

    QSerialPort *m_com;
    QByteArray m_dataGeted;

    ComTab *m_comTab;
    OscilloscopeTab *m_oscilloscope;
    LinearCcdTab *m_linearCdd;
    CcdTab *m_ccd;
};

#endif // MAINWINDOW_H
