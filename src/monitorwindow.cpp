#include "monitorwindow.h"
#include "ui_monitorwindow.h"
#include "global.h"

#include <QDateTime>
#include <QChartView>
#include <QLineSeries>
#include <QDebug>

MonitorWindow::MonitorWindow(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::MonitorWindow)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::NonModal);

    this->m_splineSeries = new QSplineSeries();
    this->m_axisX = new QDateTimeAxis();
    this->m_axisY = new QValueAxis();

    this->m_chart = new QChart();

    this->m_timer = new QTimer();
    this->m_timer->setTimerType(Qt::PreciseTimer);

    this->m_axisX->setFormat("hh:mm:ss.zzz");
    this->m_axisX->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(MAX_QUEUE_SIZE / ECG_DATA_PER_SECS - 1));
    this->m_axisX->setTickCount(MAX_QUEUE_SIZE / ECG_DATA_PER_SECS + 1);
    this->m_axisX->setTitleText(tr("time"));

    this->m_axisY->setRange(0, 4096);
    this->m_axisY->setTickCount(17);
    this->m_axisY->setTitleText(tr("power"));

    this->m_chart->setTitle(tr("ECG graph"));
    this->m_chart->addSeries(this->m_splineSeries);
    this->m_chart->addAxis(this->m_axisX, Qt::AlignBottom);
    this->m_chart->addAxis(this->m_axisY, Qt::AlignLeft);

    QList<QString> theme_list;
    theme_list << "BlueCerulean"
               << "BlueIcy"
               << "BlueNcs"
               << "BrownSand"
               << "Dark"
               << "HighContrast"
               << "Light"
               << "Qt";
    for (int i = 0; i < theme_list.size(); i++)
    {
        this->ui->theme_cb->addItem(theme_list[i]);
    }
    this->ui->theme_cb->setCurrentIndex(7);

    /* after addSeries*/
    this->m_splineSeries->attachAxis(this->m_axisX);
    this->m_splineSeries->attachAxis(this->m_axisY);

    ui->graphicsView->setChart(this->m_chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    this->m_timer->start(1000 / ECG_DATA_PER_SECS);

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTimeOut()));
    connect(this->ui->theme_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(OnChangeTheme(int)));
}

MonitorWindow::~MonitorWindow()
{
    delete ui;

    if (nullptr != this->m_splineSeries)
    {
        delete this->m_splineSeries;
        this->m_splineSeries = nullptr;
    }
    if (nullptr != this->m_axisX)
    {
        delete this->m_axisX;
        this->m_axisX = nullptr;
    }
    if (nullptr != this->m_axisY)
    {
        delete this->m_axisY;
        this->m_axisY = nullptr;
    }
    if (nullptr != this->m_chart)
    {
        delete this->m_chart;
        this->m_chart = nullptr;
    }
    if (nullptr != this->m_timer)
    {
        delete this->m_timer;
        this->m_timer = nullptr;
    }
}

void MonitorWindow::OnTimerTimeOut()
{
    if (this->ui->radioButton->isChecked())
    {
        if (this->m_splineSeries->count() >= MAX_QUEUE_SIZE)
        {
            this->m_splineSeries->removePoints(0, 1);
        }
        qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        this->m_splineSeries->append(timestamp, g_data_unit.load(std::memory_order_relaxed));
        this->m_axisX->setRange(QDateTime::fromMSecsSinceEpoch(this->m_splineSeries->at(0).x()), QDateTime::fromMSecsSinceEpoch(this->m_splineSeries->at(this->m_splineSeries->count() - 1).x()));
    }
}

void MonitorWindow::OnChangeTheme(int idx)
{
    switch (idx)
    {
    case 0:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeBlueCerulean);
        break;
    case 1:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeBlueIcy);
        break;
    case 2:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeBlueNcs);
        break;
    case 3:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeBrownSand);
        break;
    case 4:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeDark);
        break;
    case 5:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeHighContrast);
        break;
    case 6:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeLight);
        break;
    case 7:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeQt);
        break;
    default:
        this->m_chart->setTheme(QChart::ChartTheme::ChartThemeQt);
        break;
    }
}
