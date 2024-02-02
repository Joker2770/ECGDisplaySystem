#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include <QWidget>
#include <QTimer>

#include "serialport_io.h"
#include "customs.h"

namespace Ui
{
    class SettingWindow;
}

class SettingWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

public:
    bool g_bSerialPortOpen;

public slots:
    void updateAvialabeSerialPorts();

protected slots:
    void updatePort(const QString &_sPort);
    void updateBaudRate(int _idx);
    void updateDataBits(int _idx);
    void updateStopBits(int _idx);
    void updateParity(int _idx);
    void updateFlowControl(int _idx);
    void updateLanguage(int _idx);
    void updateFontSize();
    void OnRead();
    void OnOpen();
    void OnIsOpen();
    void OnClose();

private:
    Ui::SettingWindow *ui;
    SerialPortIO *serial_port_io;

    QTimer *m_timer;

    Customs *m_customs;
};

#endif // SETTINGWINDOW_H
