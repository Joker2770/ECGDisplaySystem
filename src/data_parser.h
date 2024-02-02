/*************************************************************************
    > File Name: data_parser.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月15日 星期五 09时07分35秒
 ************************************************************************/

#ifndef __DATA_PARSER_H__
#define __DATA_PARSER_H__

#include "global.h"

#include <QString>

#include <vector>
#include <iostream>
using namespace std;

class DataParser
{
public:
    DataParser();
    virtual ~DataParser();

    void waveDataJoint(const QByteArray &data);
    void vertexDataJoint(const QByteArray &data);

    vector<uint16_t> getPlotSeries(const QByteArray &data);
    pair<uint16_t, uint16_t> getPointSeries(const QByteArray &data);

public:
    vector<uint16_t> m_vPlotSeries;
    pair<uint16_t, uint16_t> m_pVertexSeries;

private:
    QByteArray m_qbaWave;
    QByteArray m_qbaVertex;
    QByteArray m_qbaWaveSeries;
    QByteArray m_qbaVertexSeries;

    bool m_bHandleWave;
};

#endif
