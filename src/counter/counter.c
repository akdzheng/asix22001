/*=============================================================================
 * Module Name: counter.c
 * Purpose:
 * Author: zhengweihua
 * Date: 2012-03-02
 * Notes: 用于模块内部的各种统计数据
 *
 *=============================================================================*/

#include <string.h>

#include "counter.h"

GCOUNTER_DATA XDATA g_counter;

void COUNTER_Init()
{
	memset(&g_counter, 0, sizeof(GCOUNTER_DATA));
}