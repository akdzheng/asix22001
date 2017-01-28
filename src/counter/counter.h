#ifndef __COUNTER_H__
#define __COUNTER_H__

#include "types.h"

typedef struct _GCOUNTER_DATA
{
    U32_T	connectTry;
    U32_T	connectSuccess;
    U32_T	connectFailed;
	U32_T	connectClosed;
}GCOUNTER_DATA;

extern GCOUNTER_DATA XDATA g_counter;

#define COUNTER_INC_CONNECT_TRY			g_counter.connectTry++
#define COUNTER_INC_CONNECT_SUCCESS		g_counter.connectSuccess++
#define COUNTER_INC_CONNECT_FAILED		g_counter.connectFailed++
#define COUNTER_INC_CONNECT_CLOSED		g_counter.connectClosed++

#define COUNTER_GET_CONNECT_TRY			g_counter.connectTry
#define COUNTER_GET_CONNECT_SUCCESS		g_counter.connectSuccess
#define COUNTER_GET_CONNECT_FAILED		g_counter.connectFailed
#define COUNTER_GET_CONNECT_CLOSED		g_counter.connectClosed

void COUNTER_Init();


#endif		//end of #ifndef __COUNTER_H__
