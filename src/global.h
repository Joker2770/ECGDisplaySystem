/*************************************************************************
    > File Name: global.h
    > Author: Jintao Yang
    > Mail: 18608842770@163.com
    > Created Time: 2023年12月18日 星期一 11时38分10秒
 ************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define MAX_QUEUE_SIZE (512)
#define MAX_SERIES_SIZE (1024)

#include<vector>
#include<iostream>
using namespace std;

extern vector<uint32_t> gVecDataQueue;
extern vector<uint32_t> gVecDataSeries;

#endif
