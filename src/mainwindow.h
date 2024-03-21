#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QCloseEvent>
#include <QTimer>
#include <QLabel>

#include "homewindow.h"
#include "monitorwindow.h"
#include "settingwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event); // 重写closeEvent函数

private slots:
    void OnTimerTimeOut();

private:
    Ui::MainWindow *ui;
    QLabel *m_label_serialport_stat;
    QLabel *m_label_size;
    QLabel *m_label_avg_f;
    QLabel *m_label_avg_u;
    QLabel *m_label_variance_f;
    QLabel *m_label_variance_u;

    QButtonGroup btnGroup;
    HomeWindow homeWnd;
    MonitorWindow monitorWnd;
    SettingWindow settingWnd;

    QTimer *m_timer;
};
#endif // MAINWINDOW_H
