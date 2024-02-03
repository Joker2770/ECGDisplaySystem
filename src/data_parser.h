/*************************************************************************
    > File Name: data_parser.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月15日 星期五 09时07分35秒
 ************************************************************************/

#ifndef __DATA_PARSER_H__
#define __DATA_PARSER_H__

#include <QString>

#include <vector>
#include <iostream>
using namespace std;

class DataParser
{
public:
    DataParser();
    virtual ~DataParser();

    void ECGDataJoint(const QByteArray &data);

    vector<uint16_t> getPlotSeries(const QByteArray &data);

public:
    vector<uint16_t> m_vPlotSeries;

private:
    QByteArray m_qbaECG;
    QByteArray m_qbaECGSeries;
};

#endif
