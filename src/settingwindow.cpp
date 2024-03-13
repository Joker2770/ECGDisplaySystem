#include "settingwindow.h"
#include "ui_settingwindow.h"
#include "global.h"

#include <QDebug>
#include <QMessageBox>

vector<uint16_t> gVecDataSeries;

SettingWindow::SettingWindow(QWidget *parent) : QWidget(parent),
                                                ui(new Ui::SettingWindow)
{
    ui->setupUi(this);

    this->setWindowModality(Qt::NonModal);

    this->serial_port_io = new SerialPortIO();
    this->m_timer = new QTimer();
    // this->m_timer->setTimerType(Qt::PreciseTimer);

    this->m_timer->start(10000);

    this->m_customs = new Customs("ECGDisplaySystem");

    this->m_data_parser = new DataParser();

    QList<QString> baud_list;
    baud_list << "1200"
              << "2400"
              << "4800"
              << "9600"
              << "19200"
              << "38400"
              << "57600"
              << "115200"
              << "230400"
              << "460800"
              << "921600";
    for (int i = 0; i < 11; i++)
    {
        ui->setting_baudrate_cb->addItem(baud_list[i]);
    }
    ui->setting_baudrate_cb->setCurrentIndex(10);

    QList<QString> data_bits_list;
    data_bits_list << "5"
                   << "6"
                   << "7"
                   << "8";
    for (int i = 0; i < 4; i++)
    {
        ui->setting_databits_cb->addItem(data_bits_list[i]);
    }
    ui->setting_databits_cb->setCurrentIndex(3);

    QList<QString> parity_list;
    parity_list << "None"
                << "Even"
                << "Odd"
                << "Space"
                << "Mark";
    for (int i = 0; i < 5; i++)
    {
        ui->setting_parity_cb->addItem(parity_list[i]);
    }
    ui->setting_parity_cb->setCurrentIndex(0);

    QList<QString> stop_list;
    stop_list << "1"
              << "1.5"
              << "2";
    for (int i = 0; i < 3; i++)
    {
        ui->setting_stopbits_cb->addItem(stop_list[i]);
    }
    ui->setting_stopbits_cb->setCurrentIndex(0);

    QList<QString> flow_control_list;
    flow_control_list << "NoFlowControl"
                      << "HardwareControl"
                      << "SoftwareControl";
    for (int i = 0; i < 3; i++)
    {
        ui->setting_flowcontrol_cb->addItem(flow_control_list[i]);
    }
    ui->setting_flowcontrol_cb->setCurrentIndex(0);

    QString qs_Font_size;
    this->m_customs->getCfgValue("view", "FontSize", qs_Font_size);
    (qs_Font_size.toInt() >= 8 && qs_Font_size.toInt() <= 18) ? this->ui->setting_font_sb->setValue(qs_Font_size.toInt()) : this->ui->setting_font_sb->setValue(12);

    QList<QString> language_list;
    language_list << "zh_CN"
                  << "en_US";
    for (int i = 0; i < 2; i++)
    {
        ui->setting_language_cb->addItem(language_list[i]);
    }
    QString q_savedLang;
    this->m_customs->getCfgValue("View", "language", q_savedLang);
    if (0 == QString::compare(q_savedLang, "zh_CN"))
        ui->setting_language_cb->setCurrentIndex(0);
    else
        ui->setting_language_cb->setCurrentIndex(1);

    this->serial_port_io->setBaudrate(921600);
    this->serial_port_io->setDataBitsLength(DATA_BITS_LEN::Data8);
    this->serial_port_io->setStopBitsLength(STOP_BITS_LEN::OneStop);
    this->serial_port_io->setParity(DATA_PARITY::NoParity);
    this->serial_port_io->setFlowControl(FLOW_CONTROL::NoFlowControl);

    connect(this->ui->setting_port_cb, SIGNAL(currentTextChanged(QString)), this, SLOT(updatePort(QString)));
    connect(this->ui->setting_baudrate_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBaudRate(int)));
    connect(this->ui->setting_databits_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDataBits(int)));
    connect(this->ui->setting_stopbits_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateStopBits(int)));
    connect(this->ui->setting_parity_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateParity(int)));
    connect(this->ui->setting_flowcontrol_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFlowControl(int)));
    connect(this->ui->setting_language_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLanguage(int)));
    connect(this->ui->setting_apply_btn, SIGNAL(clicked()), this, SLOT(updateFontSize()));
    connect(this->ui->setting_open_btn, SIGNAL(clicked()), this, SLOT(OnOpen()));
    connect(this->ui->setting_close_btn, SIGNAL(clicked()), this, SLOT(OnClose()));
    connect(this->serial_port_io->m_serial_port, SIGNAL(readyRead()), this, SLOT(OnRead()));
    connect(this->m_timer, SIGNAL(timeout()), this, SLOT(OnIsOpen()));
}

SettingWindow::~SettingWindow()
{
    delete ui;
    if (nullptr != this->serial_port_io)
    {
        delete this->serial_port_io;
        this->serial_port_io = nullptr;
    }
    if (nullptr != this->m_timer)
    {
        delete this->m_timer;
        this->m_timer = nullptr;
    }
    if (nullptr != this->m_customs)
    {
        delete this->m_customs;
        this->m_customs = nullptr;
    }
    if (nullptr != this->m_data_parser)
    {
        delete this->m_data_parser;
        this->m_data_parser = nullptr;
    }
}

void SettingWindow::updateAvialabeSerialPorts()
{
    if (!(g_bSerialPortOpen))
    {
        QList<QString> port_name_list = this->serial_port_io->scanPort();
        ui->setting_port_cb->clear();
        for (int i = 0; i < port_name_list.size(); ++i)
        {
            ui->setting_port_cb->addItem(port_name_list.at(i));
        }
    }
}

void SettingWindow::updatePort(const QString &sPort)
{
    this->serial_port_io->setPort(sPort);
}

void SettingWindow::updateBaudRate(int idx)
{
    switch (idx)
    {
    case 0:
        this->serial_port_io->setBaudrate(1200);
        break;
    case 1:
        this->serial_port_io->setBaudrate(2400);
        break;
    case 2:
        this->serial_port_io->setBaudrate(4800);
        break;
    case 3:
        this->serial_port_io->setBaudrate(9600);
        break;
    case 4:
        this->serial_port_io->setBaudrate(19200);
        break;
    case 5:
        this->serial_port_io->setBaudrate(38400);
        break;
    case 6:
        this->serial_port_io->setBaudrate(57600);
        break;
    case 7:
        this->serial_port_io->setBaudrate(115200);
        break;
    case 8:
        this->serial_port_io->setBaudrate(230400);
        break;
    case 9:
        this->serial_port_io->setBaudrate(460800);
        break;
    case 10:
        this->serial_port_io->setBaudrate(921600);
        break;
    default:
        this->serial_port_io->setBaudrate(921600);
        break;
    }

    qDebug() << "Baud idx: " << idx;
}

void SettingWindow::updateDataBits(int idx)
{
    switch (idx)
    {
    case 0:
        this->serial_port_io->setDataBitsLength(DATA_BITS_LEN::Data5);
        break;
    case 1:
        this->serial_port_io->setDataBitsLength(DATA_BITS_LEN::Data6);
        break;
    case 2:
        this->serial_port_io->setDataBitsLength(DATA_BITS_LEN::Data7);
        break;
    case 3:
        this->serial_port_io->setDataBitsLength(DATA_BITS_LEN::Data8);
        break;
    default:
        this->serial_port_io->setDataBitsLength(DATA_BITS_LEN::Data8);
        break;
    }

    qDebug() << "DataBits idx: " << idx;
}

void SettingWindow::updateStopBits(int idx)
{
    switch (idx)
    {
    case 0:
        this->serial_port_io->setStopBitsLength(STOP_BITS_LEN::OneStop);
        break;
    case 1:
        this->serial_port_io->setStopBitsLength(STOP_BITS_LEN::OneAndHalfStop);
        break;
    case 2:
        this->serial_port_io->setStopBitsLength(STOP_BITS_LEN::TwoStop);
        break;
    default:
        this->serial_port_io->setStopBitsLength(STOP_BITS_LEN::OneStop);
        break;
    }

    qDebug() << "StopBits idx: " << idx;
}

void SettingWindow::updateParity(int idx)
{
    switch (idx)
    {
    case 0:
        this->serial_port_io->setParity(DATA_PARITY::NoParity);
        break;
    case 1:
        this->serial_port_io->setParity(DATA_PARITY::EvenParity);
        break;
    case 2:
        this->serial_port_io->setParity(DATA_PARITY::OddParity);
        break;
    case 3:
        this->serial_port_io->setParity(DATA_PARITY::SpaceParity);
        break;
    case 4:
        this->serial_port_io->setParity(DATA_PARITY::MarkParity);
        break;
    default:
        this->serial_port_io->setParity(DATA_PARITY::NoParity);
        break;
    }

    qDebug() << "Parity idx: " << idx;
}

void SettingWindow::updateFlowControl(int idx)
{
    switch (idx)
    {
    case 0:
        this->serial_port_io->setFlowControl(FLOW_CONTROL::NoFlowControl);
        break;
    case 1:
        this->serial_port_io->setFlowControl(FLOW_CONTROL::HardwareControl);
        break;
    case 2:
        this->serial_port_io->setFlowControl(FLOW_CONTROL::SoftwareControl);
        break;
    default:
        this->serial_port_io->setFlowControl(FLOW_CONTROL::NoFlowControl);
        break;
    }

    qDebug() << "Flow Control idx: " << idx;
}

void SettingWindow::updateLanguage(int idx)
{
    if (0 == idx)
    {
        qDebug() << "Choose zh_CN!";
        this->m_customs->setCfgValue("View", "language", "zh_CN");
        QMessageBox::information(this, tr("Tips"), tr("Take effect after restart program!"));
    }
    else if (1 == idx)
    {
        qDebug() << "Choose en_US!";
        this->m_customs->setCfgValue("View", "language", "en_US");
        QMessageBox::information(this, tr("Tips"), tr("Take effect after restart program!"));
    }
    else
    {
        // do nothing
    }
}

void SettingWindow::updateFontSize()
{
    int value = this->ui->setting_font_sb->value();
    qDebug() << "set font size: " << value;
    this->m_customs->setCfgValue("View", "FontSize", value);
    QMessageBox::information(this, tr("Tips"), tr("Take effect after restart program!"));
}

void SettingWindow::OnRead()
{
    QByteArray qBA;
    qBA.clear();
    qBA = this->serial_port_io->readData();
    // qDebug() << "qBa: " << qBA;

    this->m_data_parser->ECGDataJoint(qBA);
}

void SettingWindow::OnOpen()
{
    g_bSerialPortOpen = this->serial_port_io->openPort();
    qDebug() << "isOpen: " << g_bSerialPortOpen;
}

void SettingWindow::OnClose()
{
    this->serial_port_io->closePort();
    g_bSerialPortOpen = false;
}

void SettingWindow::OnIsOpen()
{
    g_bSerialPortOpen = this->serial_port_io->isPortOpening();
    // qDebug() << "isOpen: " << g_bSerialPortOpen;

    this->updateAvialabeSerialPorts();
}
