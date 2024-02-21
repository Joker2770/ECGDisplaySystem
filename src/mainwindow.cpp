#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QIcon>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::NonModal);

    this->setWindowIcon(QIcon(":/icons/icon.ico"));
    this->setWindowIconText(tr("ECG with AD8232"));

    // this->showFullScreen();
    // this->setWindowFlags((windowFlags() | Qt::WindowStaysOnTopHint));

    this->m_label_serialport_stat = new QLabel("serial port status", this);
    this->m_label_size = new QLabel("out size", this);

    this->m_timer = new QTimer();
    this->m_timer->setTimerType(Qt::PreciseTimer);
    this->m_timer->start(300);

    ui->stackedWidget->addWidget(&homeWnd);
    ui->stackedWidget->addWidget(&monitorWnd);
    ui->stackedWidget->addWidget(&settingWnd);

    btnGroup.addButton(ui->btnHome, 0);
    btnGroup.addButton(ui->btnMonitor, 1);
    btnGroup.addButton(ui->btnSetting, 2);

    btnGroup.button(0)->setChecked(true);
    ui->stackedWidget->setCurrentIndex(0);

    this->ui->statusBar->addWidget(this->m_label_serialport_stat, 1);
    this->ui->statusBar->addWidget(this->m_label_size, 1);

    connect(&btnGroup, &QButtonGroup::idClicked, ui->stackedWidget, &QStackedWidget::setCurrentIndex);
    // connect(this->ui->btnSetting, SIGNAL(clicked()), &settingWnd, SLOT(updateAvialabeSerialPorts()));
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(OnTimerTimeOut()));
}

MainWindow::~MainWindow()
{
    delete ui;

    if (nullptr != this->m_timer)
    {
        delete this->m_timer;
        this->m_timer = nullptr;
    }
    if (nullptr != this->m_label_serialport_stat)
    {
        delete this->m_label_serialport_stat;
        this->m_label_serialport_stat = nullptr;
    }
    if (nullptr != this->m_label_size)
    {
        delete this->m_label_size;
        this->m_label_size = nullptr;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton button = QMessageBox::question(this, tr("Tips"), tr("Confirm quit?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (button == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::OnTimerTimeOut()
{
    this->m_label_size->setText(tr("out size:") + QString::number(this->homeWnd.m_out_text_size));
    // qDebug() << "isOpen: " << settingWnd.g_bSerialPortOpen;
    if (settingWnd.g_bSerialPortOpen)
    {
        this->m_label_serialport_stat->setStyleSheet("QLabel{color: blue;}");
        this->m_label_serialport_stat->setText(tr("serial port connected!"));
    }
    else
    {
        this->m_label_serialport_stat->setStyleSheet("QLabel{color: red;}");
        this->m_label_serialport_stat->setText(tr("serial port disconnected!"));
    }
}
