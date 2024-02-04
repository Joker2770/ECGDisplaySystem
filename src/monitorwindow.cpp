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
    this->m_axisX = new QValueAxis();
    this->m_axisY = new QValueAxis();

    this->m_chart = new QChart();

    this->m_timer = new QTimer();
    this->m_timer->setTimerType(Qt::PreciseTimer);

    this->m_axisX->setRange(QDateTime::currentDateTime().toMSecsSinceEpoch(), QDateTime::currentDateTime().addSecs(MAX_QUEUE_SIZE - 1).toMSecsSinceEpoch());
    this->m_axisX->setTitleText(tr("position"));

    this->m_axisY->setRange(0, 4096);
    this->m_axisY->setTitleText(tr("frequency"));

    this->m_chart->setTitle(tr("wave graph"));
    this->m_chart->addSeries(this->m_splineSeries);
    this->m_chart->addAxis(this->m_axisX, Qt::AlignBottom);
    this->m_chart->addAxis(this->m_axisY, Qt::AlignLeft);

    /* after addSeries*/
    this->m_splineSeries->attachAxis(this->m_axisX);
    this->m_splineSeries->attachAxis(this->m_axisY);

    ui->graphicsView->setChart(this->m_chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    this->m_timer->start(100);

    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTimeOut()));
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
        if (this->m_splineSeries->count() >= MAX_QUEUE_SIZE) {
            this->m_splineSeries->removePoints(0, 1);
        }
        qint64 timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
        this->m_splineSeries->append(timestamp, g_data_unit.load(std::memory_order_relaxed));
        this->m_axisX->setRange(this->m_splineSeries->at(0).x()
                                , this->m_splineSeries->at(this->m_splineSeries->count() - 1).x());
    }
}
