/*=============================================================================
 * Module Name: counter.c
 * Purpose:
 * Author: zhengweihua
 * Date: 2012-03-02
 * Notes: ����ģ���ڲ��ĸ���ͳ������
 *
 *=============================================================================*/

#include <string.h>

#include "counter.h"

GCOUNTER_DATA XDATA g_counter;

void COUNTER_Init()
{
	memset(&g_counter, 0, sizeof(GCOUNTER_DATA));
}