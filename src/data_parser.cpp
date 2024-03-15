/*************************************************************************
    > File Name: data_parser.cpp
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月15日 星期五 09时07分42秒
 ************************************************************************/

#include "data_parser.h"

#include "global.h"

#include <QDebug>

DataParser::DataParser()
{
    this->m_qbaECG.clear();
    this->m_qbaECGSeries.clear();
    this->m_vPlotSeries.clear();
}

DataParser::~DataParser()
{
}

void DataParser::ECGDataJoint(const QByteArray &data)
{
    if (!(data.isEmpty()))
    {
            if (this->m_qbaECG.isEmpty())
            {
                int pos = data.indexOf((char)(0xEC));
                while (-1 != pos)
                {
                    this->m_qbaECG.append(data.mid(pos, ECG_DATA_PACK_LEN - this->m_qbaECG.length()));
                    if (ECG_DATA_PACK_LEN == this->m_qbaECG.length())
                    {
                        this->getPlotSeries(this->m_qbaECG);
                        this->m_qbaECG.clear();
                    }
                    else
                    {
                        // do nothiing
                    }
                    pos = data.indexOf((char)(0xEC), pos + ECG_DATA_PACK_LEN);
                }
            }
            else
            {
                this->m_qbaECG.append(data.mid(0, ECG_DATA_PACK_LEN - this->m_qbaECG.length()));
                if (ECG_DATA_PACK_LEN == this->m_qbaECG.length())
                {
                    this->getPlotSeries(this->m_qbaECG);
                    this->m_qbaECG.clear();
                }
                else
                {
                    qDebug() << "qbaVertex error!!!";
                }
            }
    }
    else
    {
        this->m_qbaECG.clear();
        this->m_qbaECGSeries.clear();
        // g_vecVertexSeries.clear();
    }
}

std::vector<uint16_t> DataParser::getPlotSeries(const QByteArray &data)
{
    this->m_qbaECGSeries = data.mid(1, 2);
    if (this->m_qbaECGSeries.length() == ECG_DATA_SERIES_LEN)
    {
        if ((unsigned char)(data.at(ECG_DATA_PACK_LEN - 1)) == ((unsigned char)(data.at(ECG_DATA_PACK_LEN - 2))^(unsigned char)(data.at(ECG_DATA_PACK_LEN - 3))))
        {
            uint16_t ECG_unit_value = (uint16_t)((this->m_qbaECGSeries.at(1) & 0xff) | (this->m_qbaECGSeries.at(0) << 8));
            // qDebug() << ECG_unit_value;
            this->m_vPlotSeries.clear();
            this->m_vPlotSeries.push_back(ECG_unit_value);

            if (gVecDataSeries.size() < MAX_SERIES_SIZE)
                gVecDataSeries.push_back(ECG_unit_value);
        }
        else
        {
            qDebug() << data.at(ECG_DATA_PACK_LEN - 1);
        }
    }

    this->m_qbaECGSeries.clear();

    return this->m_vPlotSeries;
}
