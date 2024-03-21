/*************************************************************************
    > File Name: global.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月18日 星期一 11时38分10秒
 ************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define MAX_QUEUE_SIZE (64)
#define MAX_SERIES_SIZE (1024)
#define ECG_DATA_PACK_LEN (4)
#define ECG_DATA_SERIES_LEN (2)
#define ECG_DATA_PER_SECS (10)

#include <vector>
#include <atomic>

// extern vector<uint16_t> gVecDataQueue;
extern std::vector<uint16_t> gVecDataSeries;
extern std::atomic_char16_t g_data_unit;

#endif
