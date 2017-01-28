/*
 ******************************************************************************
 *     Copyright (c) 2011	ASIX Electronic Corporation      All rights reserved.
 *
 *     This is unpublished proprietary source code of ASIX Electronic Corporation
 *
 *     The copyright notice above does not evidence any actual or intended
 *     publication of such source code.
 ******************************************************************************
 */
 /*============================================================================
 * Module Name: gudpuc.c
 * Purpose:
 * Author:
 * Date:
 * Notes:
 * $Log: gudpuc.c,v $
 *
 *=============================================================================
 */

/* INCLUDE FILE DECLARATIONS */
#include <string.h>
#include "ax22000.h"
#include "mcpu.h"
#include "adapter.h"
#include "gudpuc.h"
#include "gs2w.h"
#include "tcpip.h"
#include "mstimer.h"
#include "stoe.h"
#include "uart0.h"
#include "gconfig.h"
#include "mac.h"
#include "printd.h"
#include "gudpdat.h"
#include "gtcpdat.h"
#include "hsuart.h"
#include "hsuart2.h"


/* NAMING CONSTANT DECLARATIONS */
/* signature(8) commmand(1) option(1) devname(16) macAddr(6) */
#define GUDPUC_AUTHENTIC_HEADER_LEN 32
#define GUDPUC_MAX_CONNS			1
#define GUDPUC_NO_NEW_CONN			0xFF

#define GUDPUC_STATE_FREE			0
#define	GUDPUC_STATE_WAIT			1
#define	GUDPUC_STATE_CONNECTED		2

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */
static GUDPUC_CONN gudpuc_Conns[GUDPUC_MAX_CONNS];
static U8_T gudpuc_InterAppId;
GCONFIG_CFG_PKT gudpuc_ConfigRxPkt;
GCONFIG_MONITOR_PKT *gudpuc_MonitorPkt;

/* LOCAL SUBPROGRAM DECLARATIONS */
void gudpuc_HandleSearchReq(U8_T id);
void gudpuc_HandleSetReq(U8_T XDATA* pData, U16_T length, U8_T id);
void gudpuc_HandleUpgradeReq(U8_T XDATA* pData, U16_T length, U8_T id);
void gudpuc_HandleResetReq(U8_T id);
void gudpuc_HandleRebootReq(U8_T XDATA* pData, U16_T length, U8_T id);
void gudpuc_RebootDevice(void);
void gudpuc_HandleMonitorReq(U8_T id);
void gudpuc_HandleConnCloseReq(U8_T XDATA *pData);


/*
 * ----------------------------------------------------------------------------
 * Function Name: GUDPUC_Task
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void GUDPUC_Task(void)
{

} /* End of GUDPUC_Task() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: GUDPUC_Init()
 * Purpose: Initialization
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void GUDPUC_Init(U16_T localPort)
{
	U8_T	i;

	printd("UDP unicast init ok.\n\r");

	for (i = 0; i < GUDPUC_MAX_CONNS; i++)
		gudpuc_Conns[i].State = GUDPUC_STATE_FREE;

	gudpuc_InterAppId = TCPIP_Bind(GUDPUC_NewConn, GUDPUC_Event, GUDPUC_Receive);
	
	/* unicast packet */
	TCPIP_UdpListen(localPort, gudpuc_InterAppId);
} /* End of GUDPUC_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: GUDPUC_NewConn
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T GUDPUC_NewConn(U32_T XDATA* pip, U16_T remotePort, U8_T socket)
{
	U8_T	i;

	printd("udp connection, socket=%bu, remoteport=%u, ip=%lu\r\n", socket, remotePort, *pip);
	for (i = 0; i < GUDPUC_MAX_CONNS; i++)
	{
		gudpuc_Conns[i].State = GUDPUC_STATE_CONNECTED;
		gudpuc_Conns[i].UdpSocket = socket;
		printd("      connect id = %bu\r\n", i);
		return i;
	}
	
	return GUDPUC_NO_NEW_CONN;

} /* End of GUDPUC_NewConn() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: GUDPUC_Event
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void GUDPUC_Event(U8_T id, U8_T event)
{
    printd("udp event, id=%bu, event=%bu\r\n", id, event);
	gudpuc_Conns[id].State = event;
} /* End of GUDPUC_Event() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: GUDPUC_Receive
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void GUDPUC_Receive(U8_T XDATA* pData, U16_T length, U8_T id)
{
	BOOL bValidReq = FALSE;
	GCONFIG_MAC_ADDR macAddr;

	printd("udp receive, id=%bu, length=%u\r\n", id, length);
	if (length >= 9 && length <= sizeof(gudpuc_ConfigRxPkt)) // 9: search request packet
	{
		memcpy(&gudpuc_ConfigRxPkt, pData, length);

		if (memcmp(&GCONFIG_Gid, pData, sizeof(GCONFIG_GID)) == 0) // Valid data
		{
			macAddr = GCONFIG_GetMacAddress();	

			if (length > GUDPUC_AUTHENTIC_HEADER_LEN)
			{
				if (memcmp(&gudpuc_ConfigRxPkt.MacAddr, &macAddr, sizeof(macAddr)) == 0)
					bValidReq = TRUE;
			}

			switch (gudpuc_ConfigRxPkt.Opcode)
			{
			case GCONFIG_OPCODE_SEARCH_REQ:
				{
					gudpuc_HandleSearchReq(id);
				}
				break;

			case GCONFIG_OPCODE_SET_REQ:
				{
					if (bValidReq && length == sizeof(gudpuc_ConfigRxPkt))
						gudpuc_HandleSetReq(pData, length, id);
				}
				break;

			case GCONFIG_OPCODE_UPGRADE_REQ:
				{
					if (bValidReq && length == sizeof(GCONFIG_UPGRADE_PKT))
						gudpuc_HandleUpgradeReq(pData, length, id);
				}
				break;

			case GCONFIG_OPCODE_RESET_REQ:
				{
					if (bValidReq)
						gudpuc_HandleResetReq(id);
				}
				break;
			case GCONFIG_OPCODE_REBOOT_REQ:
				{
					if (bValidReq)
						gudpuc_HandleRebootReq(pData, length, id);
				}
				break;
            case GCONFIG_OPCODE_MONITOR_REQ:
				{
                    gudpuc_HandleMonitorReq(id);
				}
				break;
			case GCONFIG_OPCODE_CONN_CLOSE:
				{
                    gudpuc_HandleConnCloseReq(pData);
				}
				break;
			default:
				break;	
			}
		}
	}

    TCPIP_UdpClose(gudpuc_Conns[id].UdpSocket);

} /* End of GUDPUC_Receive() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleSearchReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleSearchReq(U8_T id) 
{	
    printd("udp handle search request, size=%u\r\n", sizeof(gudpuc_ConfigRxPkt));
	GCONFIG_GetConfigPacket(&gudpuc_ConfigRxPkt);
 	gudpuc_ConfigRxPkt.Opcode = GCONFIG_OPCODE_SEARCH_ACK;
	TCPIP_UdpSend(gudpuc_Conns[id].UdpSocket, 0, 0, (U8_T*) &gudpuc_ConfigRxPkt, sizeof(gudpuc_ConfigRxPkt));
} /* End of gudpuc_HandleSearchReq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleSetReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleSetReq(U8_T XDATA* pData, U16_T length, U8_T id)
{
	U8_T reboot = (gudpuc_ConfigRxPkt.Option & GCONFIG_OPTION_ENABLE_REBOOT);

    printd("udp handle set request\r\n");

	GCONFIG_SetConfigPacket(&gudpuc_ConfigRxPkt);
	*(pData + GCONFIG_OPCODE_OFFSET) = GCONFIG_OPCODE_SET_ACK;
	TCPIP_UdpSend(gudpuc_Conns[id].UdpSocket, 0, 0, pData, length);

	if (reboot == GCONFIG_OPTION_ENABLE_REBOOT)
	{
		gudpuc_RebootDevice();
	}	
} /* End of gudpuc_HandleSetReq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleUpgradeReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleUpgradeReq(U8_T XDATA* pData, U16_T length, U8_T id)
{	
    GCONFIG_UPGRADE_PKT *pCfg = (GCONFIG_UPGRADE_PKT *)pData;

    printd("udp handle upgradate request\r\n");

    /* Get tftp server ip */
     GCONFIG_SetTftpServerIp(pCfg->TftpSrvIp, pCfg->FirmwareType);

    /* point to filename */
    GCONFIG_SetFilename(pCfg->Filename, strlen(pCfg->Filename), pCfg->FirmwareType);

	GCONFIG_EnableFirmwareUpgrade(pCfg->FirmwareType);
	GCONFIG_WriteConfigData();
	*(pData + GCONFIG_OPCODE_OFFSET) = GCONFIG_OPCODE_UPGRADE_ACK;  	
	TCPIP_UdpSend(gudpuc_Conns[id].UdpSocket, 0, 0, pData, length);
	gudpuc_RebootDevice();
} /* End of gudpuc_HandleUpgradeReq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleResetReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleResetReq(U8_T id)
{
	U8_T reboot = (gudpuc_ConfigRxPkt.Option & GCONFIG_OPTION_ENABLE_REBOOT);
	
    printd("udp handle reset request\r\n");

	GCONFIG_ReadDefaultConfigData();
	GCONFIG_WriteConfigData();
	GCONFIG_GetConfigPacket(&gudpuc_ConfigRxPkt);
 	gudpuc_ConfigRxPkt.Opcode = GCONFIG_OPCODE_RESET_ACK;
	TCPIP_UdpSend(gudpuc_Conns[id].UdpSocket, 0, 0, (U8_T*) &gudpuc_ConfigRxPkt, sizeof(gudpuc_ConfigRxPkt));

	if (reboot == GCONFIG_OPTION_ENABLE_REBOOT)
	{
		gudpuc_RebootDevice();
	}	
} /* End of gudpuc_HandleResetReq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleRebootReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleRebootReq(U8_T XDATA* pData, U16_T length, U8_T id)
{
    printd("udp handle reboot request\r\n");

	*(pData + GCONFIG_OPCODE_OFFSET) = GCONFIG_OPCODE_REBOOT_ACK;  	
	TCPIP_UdpSend(gudpuc_Conns[id].UdpSocket, 0, 0, pData, length);
	gudpuc_RebootDevice(); 
}
/* End of gudpuc_HandleRebootReq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_RebootDevice
 * Purpose: Delay 10 ms before reboot
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_RebootDevice(void)
{
	U32_T timeStart = SWTIMER_Tick();
	U32_T timeEnd = timeStart;	

    printd("******* gudpuc_RebootDevice()\r\n");

	while (timeEnd == timeStart)
	{
		timeEnd = SWTIMER_Tick();
	}

	MCPU_SoftReboot();
} /* End of gudpuc_RebootDevice() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleMonitorReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleMonitorReq(U8_T id)
{
    printd("udp handle moniter request\r\n");


    gudpuc_MonitorPkt = (GCONFIG_MONITOR_PKT *)&gudpuc_ConfigRxPkt;
	
	gudpuc_MonitorPkt->Gid = GCONFIG_Gid;
    gudpuc_MonitorPkt->Opcode = GCONFIG_OPCODE_MONITOR_ACK;
    gudpuc_MonitorPkt->ModemStatus = HSUR2_GetModemStatus(1);
    strcpy((char *)gudpuc_MonitorPkt->VerStr , GCONFIG_VERSION_STRING);
    if (GS2W_CONN_UDP == GS2W_GetConnType())
    {
        gudpuc_MonitorPkt->TxBytes = GUDPDAT_GetTxBytes(0);
        gudpuc_MonitorPkt->RxBytes = GUDPDAT_GetRxBytes(0);
    }
    else
    {
        gudpuc_MonitorPkt->TxBytes = GTCPDAT_GetTxBytes(0);
        gudpuc_MonitorPkt->RxBytes = GTCPDAT_GetRxBytes(0);
    }

	TCPIP_UdpSend(gudpuc_Conns[id].UdpSocket, 0, 0, (U8_T *)gudpuc_MonitorPkt, sizeof(GCONFIG_MONITOR_PKT));

} /* End of gudpuc_HandleMonitorReq() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: gudpuc_HandleConnCloseReq
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void gudpuc_HandleConnCloseReq(U8_T XDATA *pData)
{
    GCONFIG_CONN_CLOSE_PKT *pCfg = (GCONFIG_CONN_CLOSE_PKT *)pData;

    printd("udp handle connection close\r\n");


    if (GS2W_CONN_UDP == GS2W_GetConnType())
    {
        GUDPDAT_ConnClose(pCfg->RemoteIp, pCfg->Port);
    }
    else
    {
        GTCPDAT_ConnClose(pCfg->RemoteIp, pCfg->Port);
    }
} /* End of gudpuc_HandleConnCloseReq() */

/* End of gudpuc.c */
