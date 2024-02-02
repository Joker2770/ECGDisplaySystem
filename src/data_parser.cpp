/*************************************************************************
    > File Name: data_parser.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月15日 星期五 09时07分42秒
 ************************************************************************/

#include "data_parser.h"
#include "crc16_modbus.h"

#include <QDebug>

vector<uint16_t> g_VecWaveSeries;
vector<pair<uint16_t, uint16_t>> g_vecVertexSeries;

DataParser::DataParser()
{
    this->m_qbaWave.clear();
    this->m_qbaWaveSeries.clear();
    this->m_qbaVertex.clear();
    this->m_qbaVertexSeries.clear();
    this->m_vPlotSeries.clear();

    this->m_bHandleWave = false;
}

DataParser::~DataParser()
{
}

void DataParser::waveDataJoint(const QByteArray &data)
{
    if (!(data.isEmpty()))
    {
        if (this->m_qbaWave.isEmpty())
        {
            int pos = data.indexOf("\xcf\xfc");
            while (-1 != pos)
            {
                this->m_bHandleWave = true;
                this->m_qbaWave.append(data.mid(pos, WAVE_PACK_LEN - this->m_qbaWave.length()));
                if (WAVE_PACK_LEN == this->m_qbaWave.length())
                {
                    this->getPlotSeries(this->m_qbaWave);
                    this->m_qbaWave.clear();
                    this->m_qbaWaveSeries.clear();
                    this->m_bHandleWave = false;
                }
                else
                {
                    // do nothiing
                }
                pos = data.indexOf("\xcf\xfc", pos + WAVE_PACK_LEN);
            }
        }
        else
        {
            if (this->m_bHandleWave)
            {
                this->m_qbaWave.append(data.mid(0, WAVE_PACK_LEN - this->m_qbaWave.length()));
                if (WAVE_PACK_LEN == this->m_qbaWave.length())
                {
                    this->getPlotSeries(this->m_qbaWave);
                    this->m_qbaWave.clear();
                    this->m_qbaWaveSeries.clear();
                    this->m_bHandleWave = false;
                }
                else
                {
                    // do nothiing
                }
            }
            else
            {
                this->m_qbaWave.clear();
            }
        }
    }
    else
    {
        this->m_qbaWave.clear();
        this->m_qbaWaveSeries.clear();
        g_VecWaveSeries.clear();
    }
}

void DataParser::vertexDataJoint(const QByteArray &data)
{
    if (!(data.isEmpty()))
    {
        int pos = data.indexOf("\xcf\xfc");
        if (!(this->m_bHandleWave) && -1 == pos)
        {
            if (this->m_qbaVertex.isEmpty())
            {
                int pos = data.indexOf((char)(0xc3));
                while (-1 != pos)
                {
                    this->m_qbaVertex.append(data.mid(pos, VERTEX_PACK_LEN - this->m_qbaVertex.length()));
                    if (VERTEX_PACK_LEN == this->m_qbaVertex.length())
                    {
                        this->getPointSeries(this->m_qbaVertex);
                        this->m_qbaVertex.clear();
                        this->m_pVertexSeries.first = 0;
                        this->m_pVertexSeries.second = 0;
                    }
                    else
                    {
                        // do nothiing
                    }
                    pos = data.indexOf((char)(0xc3), pos + VERTEX_PACK_LEN);
                }
            }
            else
            {
                this->m_qbaVertex.append(data.mid(0, VERTEX_PACK_LEN - this->m_qbaVertex.length()));
                if (VERTEX_PACK_LEN == this->m_qbaVertex.length())
                {
                    this->getPointSeries(this->m_qbaVertex);
                    this->m_qbaVertex.clear();
                    this->m_pVertexSeries.first = 0;
                    this->m_pVertexSeries.second = 0;
                }
                else
                {
                    qDebug() << "qbaVertex error!!!";
                }
            }
        }
    }
    else
    {
        this->m_qbaVertex.clear();
        this->m_qbaVertexSeries.clear();
        // g_vecVertexSeries.clear();
    }
}

vector<uint16_t> DataParser::getPlotSeries(const QByteArray &data)
{
    this->m_qbaWaveSeries = data.mid(2, WAVE_SERIES_LEN);
    if (this->m_qbaWaveSeries.length() == WAVE_SERIES_LEN)
    {
        // qDebug() << this->m_qbaWaveSeries;
        unsigned short crc16 = crc16_modbus(reinterpret_cast<unsigned char *>(this->m_qbaWaveSeries.data()), this->m_qbaWaveSeries.length());
        //            qDebug() << crc16;
        //            qDebug() << (unsigned char)(crc16 >> 8);
        //            qDebug() << (unsigned char)(crc16 & 0xff);
        //            qDebug() << (unsigned char)(data.at(WAVE_PACK_LEN - 2));
        //            qDebug() << (unsigned char)(data.at(WAVE_PACK_LEN - 1));
        if ((unsigned char)(data.at(WAVE_PACK_LEN - 1)) == (unsigned char)(crc16 & 0xff) && (unsigned char)(data.at(WAVE_PACK_LEN - 2)) == (unsigned char)(crc16 >> 8))
        {
            this->m_vPlotSeries.clear();
            for (int i = 0; i < WAVE_SERIES_LEN; i += 2)
            {
                this->m_vPlotSeries.push_back((uint16_t)((this->m_qbaWaveSeries.at(i + 1) & 0xff) | (this->m_qbaWaveSeries.at(i) << 8)));
            }
            g_VecWaveSeries.clear();
            g_VecWaveSeries = this->m_vPlotSeries;
        }
        else
        {
            qDebug() << "crc16 check error!";
            qDebug() << crc16;
            qDebug() << (unsigned char)(crc16 >> 8);
            qDebug() << (unsigned char)(crc16 & 0xff);
            qDebug() << (unsigned char)(data.at(WAVE_PACK_LEN - 2));
            qDebug() << (unsigned char)(data.at(WAVE_PACK_LEN - 1));
            qDebug() << data.toHex();
        }
    }

    this->m_qbaWaveSeries.clear();

    return this->m_vPlotSeries;
}

pair<uint16_t, uint16_t> DataParser::getPointSeries(const QByteArray &data)
{
    this->m_pVertexSeries.first = 0;
    this->m_pVertexSeries.second = 0;
    this->m_qbaVertexSeries = data.mid(1, VERTEX_SERIES_LEN);
    if (this->m_qbaVertexSeries.length() == VERTEX_SERIES_LEN)
    {
        if ((unsigned char)((unsigned char)(this->m_qbaVertexSeries.at(0)) ^ (unsigned char)(this->m_qbaVertexSeries.at(1)) ^ (unsigned char)(this->m_qbaVertexSeries.at(2)) ^ (unsigned char)(this->m_qbaVertexSeries.at(3))) == (unsigned char)(data.at(VERTEX_PACK_LEN - 1)))
        {
            this->m_pVertexSeries.first = (uint16_t)((this->m_qbaVertexSeries.at(0) << 8) | (this->m_qbaVertexSeries.at(1) & 0xff));
            this->m_pVertexSeries.second = (uint16_t)((this->m_qbaVertexSeries.at(2) << 8) | (this->m_qbaVertexSeries.at(3) & 0xff));
            if (g_vecVertexSeries.size() < 1024)
            {
                g_vecVertexSeries.push_back(this->m_pVertexSeries);
            }
        }
        else
        {
            qDebug() << data.toHex();
            qDebug() << "xor check: " << (unsigned char)((unsigned char)(this->m_qbaVertexSeries.at(0)) ^ (unsigned char)(this->m_qbaVertexSeries.at(1)) ^ (unsigned char)(this->m_qbaVertexSeries.at(2)) ^ (unsigned char)(this->m_qbaVertexSeries.at(3)));
            qDebug() << "vertex data pairity: " << (unsigned char)(data.at(VERTEX_PACK_LEN - 1));
        }
    }
    this->m_qbaVertexSeries.clear();

    return this->m_pVertexSeries;
}
