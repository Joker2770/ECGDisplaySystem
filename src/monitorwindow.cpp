#include "monitorwindow.h"
#include "ui_monitorwindow.h"
#include "global.h"

#include <QChartView>
#include <QLineSeries>
#include <QDebug>

MonitorWindow::MonitorWindow(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::MonitorWindow)
{
    ui->setupUi(this);

    this->m_splineSeries = new QSplineSeries();
    this->m_axisX = new QValueAxis();
    this->m_axisY = new QValueAxis();

    this->m_chart = new QChart();

    this->m_timer = new QTimer();
    this->m_timer->setTimerType(Qt::PreciseTimer);

    this->m_axisX->setMin(0);
    this->m_axisX->setMax(MAX_QUEUE_SIZE);
    this->m_axisX->setRange(0, MAX_QUEUE_SIZE);
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
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i)
    {
        this->m_splineSeries->append(i, i);
    }

    ui->graphicsView->setChart(this->m_chart);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    this->m_timer->start(150);

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
        this->m_splineSeries->clear();

        for (size_t i = 0; i < gVecDataQueue.size(); ++i) {
            this->m_splineSeries->append(i, gVecDataQueue.at(i));
        }
    }
}
