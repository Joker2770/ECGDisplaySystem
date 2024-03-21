/*************************************************************************
    > File Name: serialport_io.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月14日 星期四 14时33分09秒
 ************************************************************************/

#include "serialport_io.h"
#include <QDebug>

SerialPortIO::SerialPortIO()
{
    this->m_serial_port = new QSerialPort();
}

SerialPortIO::~SerialPortIO()
{
    this->m_serial_port->close();
    if (nullptr != this->m_serial_port)
    {
        delete this->m_serial_port;
        this->m_serial_port = nullptr;
    }
}

QList<QString> SerialPortIO::scanPort()
{
    /* 查找可用串口 */
    QList<QString> port_list;
    port_list.clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        port_list.append(info.portName());
    }
    return port_list;
}

void SerialPortIO::setPort(const QString &port)
{
    this->m_serial_port->setPortName(port);
}

bool SerialPortIO::setBaudrate(unsigned int baudrate)
{
    return this->m_serial_port->setBaudRate(baudrate);
}

bool SerialPortIO::setDataBitsLength(DATA_BITS_LEN data_bits_len)
{
    bool bSucceed = false;
    switch (data_bits_len)
    {
    case DATA_BITS_LEN::Data5:
        bSucceed = this->m_serial_port->setDataBits(QSerialPort::Data5);
        break;
    case DATA_BITS_LEN::Data6:
        bSucceed = this->m_serial_port->setDataBits(QSerialPort::Data6);
        break;
    case DATA_BITS_LEN::Data7:
        bSucceed = this->m_serial_port->setDataBits(QSerialPort::Data7);
        break;
    case DATA_BITS_LEN::Data8:
        bSucceed = this->m_serial_port->setDataBits(QSerialPort::Data8);
        break;
    default:
        bSucceed = this->m_serial_port->setDataBits(QSerialPort::Data8);
        break;
    }
    return bSucceed;
}

bool SerialPortIO::setStopBitsLength(STOP_BITS_LEN stop_bits_len)
{
    bool bSucceed = false;
    switch (stop_bits_len)
    {
    case STOP_BITS_LEN::OneStop:
        bSucceed = this->m_serial_port->setStopBits(QSerialPort::OneStop);
        break;
    case STOP_BITS_LEN::OneAndHalfStop:
        bSucceed = this->m_serial_port->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case STOP_BITS_LEN::TwoStop:
        bSucceed = this->m_serial_port->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        bSucceed = this->m_serial_port->setStopBits(QSerialPort::OneStop);
        break;
    }
    return bSucceed;
}

bool SerialPortIO::setParity(DATA_PARITY data_parity)
{
    bool bSucceed = false;
    switch (data_parity)
    {
    case DATA_PARITY::NoParity:
        bSucceed = this->m_serial_port->setParity(QSerialPort::NoParity);
        break;
    case DATA_PARITY::EvenParity:
        bSucceed = this->m_serial_port->setParity(QSerialPort::EvenParity);
        break;
    case DATA_PARITY::OddParity:
        bSucceed = this->m_serial_port->setParity(QSerialPort::OddParity);
        break;
    case DATA_PARITY::SpaceParity:
        bSucceed = this->m_serial_port->setParity(QSerialPort::SpaceParity);
        break;
    case DATA_PARITY::MarkParity:
        bSucceed = this->m_serial_port->setParity(QSerialPort::MarkParity);
        break;
    default:
        bSucceed = this->m_serial_port->setParity(QSerialPort::NoParity);
        break;
    }
    return bSucceed;
}

bool SerialPortIO::setFlowControl(FLOW_CONTROL flow_control)
{
    bool bSucceed = false;
    switch (flow_control)
    {
    case FLOW_CONTROL::NoFlowControl:
        bSucceed = this->m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
        break;
    case FLOW_CONTROL::HardwareControl:
        bSucceed = this->m_serial_port->setFlowControl(QSerialPort::HardwareControl);
        break;
    case FLOW_CONTROL::SoftwareControl:
        bSucceed = this->m_serial_port->setFlowControl(QSerialPort::SoftwareControl);
        break;
    default:
        bSucceed = this->m_serial_port->setFlowControl(QSerialPort::NoFlowControl);
        break;
    }
    return bSucceed;
}

bool SerialPortIO::openPort()
{
    this->m_serial_port->setReadBufferSize(1024);
    return this->m_serial_port->open(QIODevice::ReadWrite);
}

bool SerialPortIO::isPortOpening()
{
    return this->m_serial_port->isOpen();
}

void SerialPortIO::closePort()
{
    this->m_serial_port->clear();
    this->m_serial_port->close();
}

QByteArray SerialPortIO::readData()
{
    return this->m_serial_port->readAll();
}

int SerialPortIO::writeData(const QString &data)
{
    return this->m_serial_port->write(data.toStdString().c_str());
}
