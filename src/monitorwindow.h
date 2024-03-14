#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QWidget>
#include <QChartView>
#include <QSplineSeries>
#include <QValueAxis>
#include <QTimer>
#include <QtCharts>

#include <iostream>
using namespace std;

QT_CHARTS_USE_NAMESPACE

namespace Ui
{
    class MonitorWindow;
}

class MonitorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MonitorWindow(QWidget *parent = nullptr);
    ~MonitorWindow();

private slots:
    void OnTimerTimeOut();

private:
    Ui::MonitorWindow *ui;

    QSplineSeries *m_splineSeries;

    QValueAxis *m_axisY;
    QDateTimeAxis *m_axisX;

    QChart *m_chart;

    QTimer *m_timer;
};

#endif // MONITORWINDOW_H
