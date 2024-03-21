/*************************************************************************
    > File Name: serialport_io.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月14日 星期四 14时32分58秒
 ************************************************************************/

#ifndef __SERIALPORT_IO_H__
#define __SERIALPORT_IO_H__

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

enum DATA_BITS_LEN
{
    Data5,
    Data6,
    Data7,
    Data8
};

enum STOP_BITS_LEN
{
    OneStop,
    OneAndHalfStop,
    TwoStop
};

enum FLOW_CONTROL
{
    NoFlowControl,
    HardwareControl,
    SoftwareControl
};

enum DATA_PARITY
{
    NoParity,
    EvenParity,
    OddParity,
    SpaceParity,
    MarkParity
};

class SerialPortIO
{
public:
    SerialPortIO();
    virtual ~SerialPortIO();

    QList<QString> scanPort();
    void setPort(const QString &_port);
    bool setBaudrate(unsigned int _baudrate);
    bool setDataBitsLength(DATA_BITS_LEN _data_bits);
    bool setStopBitsLength(STOP_BITS_LEN _stop_bits);
    bool setParity(DATA_PARITY _parity);
    bool setFlowControl(FLOW_CONTROL _flow_control);
    QByteArray readData();
    int writeData(const QString &_data);
    bool openPort();
    bool isPortOpening();
    void closePort();

public:
    QSerialPort *m_serial_port;
};

#endif
