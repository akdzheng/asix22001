

/* INCLUDE FILE DECLARATIONS */
#include <string.h>
#include "adapter.h"
#include "tcpip.h"
#include "stoe.h"
#include "mac.h"
#include "mstimer.h"
#include "dhcpc.h"


/* NAMING CONSTANT DECLARATIONS */
#define DHCPC_MESSAGE_NOT_FOR_ME	0
#define DHCPC_MESSAGE_FAIL			1
#define DHCPC_MESSAGE_SUCCESS		2

/* GLOBAL VARIABLES DECLARATIONS */


/* STATIC VARIABLE DECLARATIONS */
static DHCP_INFO	dhcpInfo;
static U8_T		dhcpConnID = 0;
static DHCP_HEADER	dhcpBuffer;
static U16_T		dhcp_curTime, dhcp_lastTime=0, dhcp_Time = 0;
static U8_T		dhcp_Retry = 0;
static U32_T		dhcpXid = 0;
static U8_T		dhcpMacAddr[6] = {0};
static U32_T		dhcp_LeaseTime = 0;
static U32_T		dhcp_LeaseTimerT1 = 0;

/* LOCAL SUBPROGRAM DECLARATIONS */
void dhcp_Discover(void);
void dhcp_Request(U8_T bcast);
void dhcp_OptMagicCookie(void);
void dhcp_OptionsAppend(U8_T optCode, U8_T optLen, U8_T *optContent);
U8_T dhcp_ReceiveMessage(U16_T);
void dhcp_OptionCheck(U8_T msgType);
U8_T *dhcp_GetOption(U8_T *optMsg, U8_T msgCode, U8_T *vLen, U16_T);

/*
 *--------------------------------------------------------------------------------
 * void DHCP_Init(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
BOOL DHCP_Init(void)
{
	dhcpInfo.AppId = TCPIP_Bind(NULL, DHCP_EventHandle, DHCP_ReceiveHandle); // assigned by TCP stack
	dhcpInfo.State = DHCP_IDLE_STATE;

	return TRUE;

} /* End of DHCP_Init() */

/*
 *--------------------------------------------------------------------------------
 * void DHCPC_Task(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCPC_Task(void)
{
    dhcp_curTime = (U16_T)SWTIMER_Tick();
    if ((dhcp_curTime - dhcp_lastTime) >= SWTIMER_COUNT_SECOND)
    {
        dhcp_lastTime = dhcp_curTime;
        DHCPC_LeaseTimeCheck();
    }

} /* End of DHCPC_Task() */

/*
 *--------------------------------------------------------------------------------
 * void DHCP_ResetState(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCP_ResetState(void)
{
	dhcpInfo.State = DHCP_IDLE_STATE;
} /* End of DHCP_ResetState() */

/*
 *--------------------------------------------------------------------------------
 * void DHCP_Start(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCP_Start(void)
{
	U16_T	i;
	U8_T	*point;

	if (dhcpInfo.State > DHCP_IDLE_STATE)
		return;

	/* set uip host ip address */
	TCPIP_SetIPAddr(0);

	/* set stoe host ip address */
	STOE_SetIPAddr(0);

	dhcpInfo.pOpt = (U8_T *)(dhcpBuffer.Options);
	dhcpInfo.OptionsCounter = 0;
	dhcpInfo.Secs = 0;
	dhcpInfo.TimeoutInterval = DHCP_REQUEST_TIMEOUT;
	for (i=0 ; i<4 ; i++)
	{
		dhcpInfo.ServerId[i] = 0;
		dhcpInfo.ClientOfferedIp[i] = 0;
	}
	dhcpInfo.IpAddr = 0;
	dhcpInfo.NetMask = 0;
	dhcpInfo.DnsAddr = 0;
	dhcpInfo.DefaultRouter = 0;
	dhcpInfo.LeaseTime = 0;

	dhcpInfo.State = DHCP_INITIAL_STATE;
	dhcp_Retry = 0;
	dhcp_LeaseTime = 0;
	dhcp_LeaseTimerT1 = 0;

	point = MAC_GetMacAddr();
	for (i=0 ; i<MAC_ADDRESS_LEN ; i++)
		dhcpMacAddr[i] = point[i];
	/* Transaction ID assign again with the lowest two bytes of each MAC address */
	dhcpXid = DHCP_CLIENT_XID | ((U32_T)dhcpMacAddr[4] << 8) | ((U32_T)dhcpMacAddr[5]);
	
} /* End of DHCP_Start() */

/*
 *--------------------------------------------------------------------------------
 * void DHCP_Send(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCP_Send(void)
{
	if (dhcpInfo.State == DHCP_INITIAL_STATE)
	{
		dhcp_Discover();
		dhcp_Time = (U16_T)SWTIMER_Tick();
	}
	else if (dhcpInfo.State == DHCP_RCV_OFFER_STATE)
	{
		dhcp_Request(1);
		dhcp_Time = (U16_T)SWTIMER_Tick();
	}

} /* End of DHCP_Send() */

/*
 * ----------------------------------------------------------------------------
 * void DHCP_EventHandle(U8_T connId, U8_T event)
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void DHCP_EventHandle(U8_T connId, U8_T event)
{
	if (connId != 0)
		return;

	if (event == TCPIP_CONNECT_CANCEL)
	{
		dhcpInfo.State = DHCP_IDLE_STATE;
		TCPIP_UdpClose(dhcpInfo.SocketId);
	}
}

/*
 * ----------------------------------------------------------------------------
 * void DHCP_ReceiveHandle(U8_T XDATA* pData, U16_T length, U8_T connId)
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void DHCP_ReceiveHandle(U8_T XDATA *pData, U16_T length, U8_T connId)
{
	U8_T	tmp;

	if (connId != 0)
		return;

	dhcpInfo.dhcpPktPtr = (DHCP_HEADER *)pData;

	if (dhcpInfo.State == DHCP_DISCOVER_STATE)
	{
		tmp = dhcp_ReceiveMessage(length);
		if (tmp == DHCPC_MESSAGE_SUCCESS)
		{
			dhcpInfo.State = DHCP_RCV_OFFER_STATE;
			return;
		}
		else if (tmp == DHCPC_MESSAGE_NOT_FOR_ME)
			return;
		else
			dhcpInfo.State = DHCP_INITIAL_STATE;
	}
	else if (dhcpInfo.State == DHCP_REQUEST_STATE)
	{
		tmp = dhcp_ReceiveMessage(length);
		if (tmp == DHCPC_MESSAGE_SUCCESS)
			dhcpInfo.State = DHCP_IDLE_STATE;
		else if (tmp == DHCPC_MESSAGE_NOT_FOR_ME)
			return;
		else
			dhcpInfo.State = DHCP_INITIAL_STATE;
	}
	else if (dhcpInfo.State == DHCP_RENEW_STATE)
	{
		tmp = dhcp_ReceiveMessage(length);
		if (tmp == DHCPC_MESSAGE_SUCCESS)
			dhcpInfo.State = DHCP_IDLE_STATE;
		else
            return;
	}
	else
		return;

	TCPIP_UdpClose(dhcpInfo.SocketId);

} /* End of DHCP_ReceiveHandle() */

/*
 *--------------------------------------------------------------------------------
 * void DHCP_GetState(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
S8_T DHCP_GetState(void)
{
	if (dhcpInfo.State == DHCP_DISCOVER_STATE || dhcpInfo.State == DHCP_REQUEST_STATE)
	{
		dhcp_curTime = (U16_T)SWTIMER_Tick();
		if ((dhcp_curTime - dhcp_Time) >= DHCP_REQUEST_TIMEOUT) /* had timeout */
		{
			TCPIP_UdpClose(dhcpInfo.SocketId);
			if (++dhcp_Retry < DHCP_MAX_RETRY)
				dhcpInfo.State = DHCP_INITIAL_STATE;
			else
				dhcpInfo.State = DHCP_IDLE_STATE;
		}
	}
	return dhcpInfo.State;
}

/*
 *--------------------------------------------------------------------------------
 * void dhcp_Discover(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void dhcp_Discover(void)
{
	DHCP_HEADER		*pDiscover;
	U16_T			addr[2] = {0, 0};
	U8_T			optItem[3] = {0, 0, 0};
	U16_T			i;

	/* assigned by stack */
	dhcpInfo.SocketId = TCPIP_UdpNew(dhcpInfo.AppId, dhcpConnID, DHCP_CLIENT_BROADCAST, (U16_T)DHCP_CLIENT_PORT,
					(U16_T)DHCP_SERVER_PORT);
	if (dhcpInfo.SocketId == TCPIP_NO_NEW_CONN)
	{
		return;
	}

	pDiscover = &dhcpBuffer;

	pDiscover->Op = DHCP_C_TO_S;
	pDiscover->Htype = DHCP_HTYPE_ETHERNET;
	pDiscover->Hlen = DHCP_HLEN_ETHERNET;
	pDiscover->Hops = 0;
	pDiscover->Xid = dhcpXid;
	pDiscover->Seconds = dhcpInfo.Secs;
	pDiscover->Flags = 0x8000;
	pDiscover->CiAddr = 0;
	pDiscover->YiAddr = 0;
	pDiscover->SiAddr = 0;
	pDiscover->GiAddr = 0;

	for (i=0 ; i<DHCP_CHADDR_LEN ; i++)
	{
		if (i < MAC_ADDRESS_LEN)
			pDiscover->ChAddr[i] = dhcpMacAddr[i];
		else
			pDiscover->ChAddr[i] = 0;
	}
	for (i=0 ; i<DHCP_SNAME_LEN ; i++)
	{
		pDiscover->Sname[i] = 0;
	}
	for (i=0 ; i<DHCP_FILE_LEN ; i++)
	{
		pDiscover->File[i] = 0;
	}

	dhcpInfo.OptionsCounter = 0;

	/* Add magic cookie bytes */
	dhcpInfo.pOpt = &(pDiscover->Options);
	dhcp_OptMagicCookie();

	/* Add DHCP_DISCOVER option */
	optItem[0] = DHCP_DISCOVER;
	dhcp_OptionsAppend(DHCP_OPTION_MSG_TYPE, 1, optItem);

	/* Add DHCP_OPTION_PAR_REQ_LIST option */
	optItem[0] = DHCP_OPTION_SUBNET_MASK;
	optItem[1] = DHCP_OPTION_ROUTER;
	optItem[2] = DHCP_OPTION_DNS_SERVER;
	dhcp_OptionsAppend(DHCP_OPTION_PAR_REQ_LIST, 3, optItem);

	/* Add DHCP_OPTION_END option */
	optItem[0] = 0;
	dhcp_OptionsAppend(DHCP_OPTION_END, 0, optItem);
	
	while (dhcpInfo.OptionsCounter < DHCP_MIN_OPTIONS_LEN)
	{
		pDiscover->Options[dhcpInfo.OptionsCounter ++] = 0;
	}

	TCPIP_UdpSend((dhcpInfo.SocketId), NULL, 0, (U8_T *)pDiscover, (U16_T)(DHCP_FIXED_LEN + dhcpInfo.OptionsCounter));

	dhcpInfo.State = DHCP_DISCOVER_STATE;
}

/*
 *--------------------------------------------------------------------------------
 * void dhcp_Request(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void dhcp_Request(U8_T bcast)
{
	DHCP_HEADER		*pRequest;
	U16_T			uip_addr[2] = {0, 0};
	U8_T			optItem[3] = {0, 0, 0};
	U16_T			i;

	pRequest = &dhcpBuffer;

	pRequest->Op = DHCP_C_TO_S;
	pRequest->Htype = DHCP_HTYPE_ETHERNET;
	pRequest->Hlen = DHCP_HLEN_ETHERNET;
	pRequest->Hops = 0;
	pRequest->Xid = dhcpXid;
	pRequest->Seconds = 0;
    if (bcast)
	    pRequest->Flags = 0x8000;
    else
	pRequest->Flags = 0;
	pRequest->CiAddr = dhcpInfo.IpAddr;
	pRequest->YiAddr = 0;
	pRequest->SiAddr = 0;
	pRequest->GiAddr = 0;

	for (i=0 ; i<DHCP_CHADDR_LEN ; i++)
	{
		if (i < MAC_ADDRESS_LEN)
			pRequest->ChAddr[i] = dhcpMacAddr[i];
		else
			pRequest->ChAddr[i] = 0;
	}
	for (i=0 ; i<DHCP_SNAME_LEN ; i++)
	{
		pRequest->Sname[i] = 0;
	}
	for (i=0 ; i<DHCP_FILE_LEN ; i++)
	{
		pRequest->File[i] = 0;
	}

	dhcpInfo.OptionsCounter = 0;

	/* Add magic cookie bytes */
	dhcpInfo.pOpt = &(pRequest->Options);
	dhcp_OptMagicCookie();

	/* Add DHCP_REQUEST option */
	optItem[0] = DHCP_REQUEST;
	dhcp_OptionsAppend(DHCP_OPTION_MSG_TYPE, 1, optItem);

    if (bcast)
    {
	/* Add DHCP_OPTION_REQ_IPADDR option */
	dhcp_OptionsAppend(DHCP_OPTION_REQ_IPADDR, 4, &(dhcpInfo.ClientOfferedIp[0]));

	/* Add DHCP_OPTION_SERVER_ID option */
	dhcp_OptionsAppend(DHCP_OPTION_SERVER_ID, 4, &(dhcpInfo.ServerId[0]));
    }
	/* Add DHCP_OPTION_PAR_REQ_LIST option */
	optItem[0] = DHCP_OPTION_SUBNET_MASK;
	optItem[1] = DHCP_OPTION_ROUTER;
	optItem[2] = DHCP_OPTION_DNS_SERVER;
	dhcp_OptionsAppend(DHCP_OPTION_PAR_REQ_LIST, 3, optItem);

	/* Add DHCP_OPTION_END option */
	optItem[0] = 0;
	dhcp_OptionsAppend(DHCP_OPTION_END, 0, optItem);
	while ((dhcpInfo.OptionsCounter < DHCP_MIN_OPTIONS_LEN))
	{
		pRequest->Options[dhcpInfo.OptionsCounter ++] = 0;
	}

	TCPIP_UdpSend((dhcpInfo.SocketId), NULL, 0, (U8_T *)pRequest, (U16_T)(DHCP_FIXED_LEN + dhcpInfo.OptionsCounter));

	dhcpInfo.State = DHCP_REQUEST_STATE;
}

/*
 *--------------------------------------------------------------------------------
 * void DHCP_Release(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
#if 0
void DHCP_Release(void)
{
	DHCP_HEADER		*pRelease;
	U8_T			optItem[3] = {0, 0, 0};
	U16_T			i;

	pRelease = &dhcpBuffer;

	pRelease->Op = DHCP_C_TO_S;
	pRelease->Htype = DHCP_HTYPE_ETHERNET;
	pRelease->Hlen = DHCP_HLEN_ETHERNET;
	pRelease->Hops = 0;
	pRelease->Xid = dhcpXid;
	pRelease->Seconds = 0;
	pRelease->Flags = 0;
	pRelease->CiAddr = dhcpInfo.IpAddr;
	pRelease->YiAddr = 0;
	pRelease->SiAddr = 0;
	pRelease->GiAddr = 0;
	for (i=0 ; i<DHCP_CHADDR_LEN ; i++)
	{
		if (i < MAC_ADDRESS_LEN)
			pRelease->ChAddr[i] = dhcpMacAddr[i];
		else
			pRelease->ChAddr[i] = 0;
	}
	for (i=0 ; i<DHCP_SNAME_LEN ; i++)
	{
		pRelease->Sname[i] = 0;
	}
	for (i=0 ; i<DHCP_FILE_LEN ; i++)
	{
		pRelease->File[i] = 0;
	}

	dhcpInfo.OptionsCounter = 0;

	/* Add magic cookie bytes */
	dhcpInfo.pOpt = &(pRelease->Options);
	dhcp_OptMagicCookie();

	/* Add DHCP_REQUEST option */
	optItem[0] = DHCP_RELEASE;
	dhcp_OptionsAppend(DHCP_OPTION_MSG_TYPE, 1, optItem);

	/* Add DHCP_OPTION_END option */
	optItem[0] = 0;
	dhcp_OptionsAppend(DHCP_OPTION_END, 0, optItem);
	while ((dhcpInfo.OptionsCounter < DHCP_OPTIONS_LEN))
	{
		pRelease->Options[dhcpInfo.OptionsCounter ++] = 0;
	}

	DHCP_TimeSet(3);
	DHCP_TimeStart();
}
#endif

/*
 *--------------------------------------------------------------------------------
 * void DHCP_Decline(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
#if 0
void DHCP_Decline(void)
{
	DHCP_HEADER		*pDecline;
	U8_T			optItem[3] = {0, 0, 0};
	U16_T			i;

	pDecline = &dhcpBuffer;

	pDecline->Op = DHCP_C_TO_S;
	pDecline->Htype = DHCP_HTYPE_ETHERNET;
	pDecline->Hlen = DHCP_HLEN_ETHERNET;
	pDecline->Hops = 0;
	pDecline->Xid = dhcpXid;
	pDecline->Seconds = 0;
	pDecline->Flags = 0;
	pDecline->CiAddr = 0;
	pDecline->YiAddr = 0;
	pDecline->SiAddr = 0;
	pDecline->GiAddr = 0;
	for (i=0 ; i<MAC_ADDRESS_LEN ; i++)
	{
		if (i < MAC_ADDRESS_LEN)
			pDecline->ChAddr[i] = dhcpMacAddr[i];
		else
			pDecline->ChAddr[i] = 0;
	}
	for (i=0 ; i<DHCP_SNAME_LEN ; i++)
	{
		pDecline->Sname[i] = 0;
	}
	for (i=0 ; i<DHCP_FILE_LEN ; i++)
	{
		pDecline->File[i] = 0;
	}

	dhcpInfo.OptionsCounter = 0;

	/* Add magic cookie bytes */
	dhcpInfo.pOpt = &(pDecline->Options);
	dhcp_OptMagicCookie();

	/* Add DHCP_REQUEST option */
	optItem[0] = DHCP_DECLINE;
	dhcp_OptionsAppend(DHCP_OPTION_MSG_TYPE, 1, optItem);

	/* Add DHCP_OPTION_END option */
	optItem[0] = 0;
	dhcp_OptionsAppend(DHCP_OPTION_END, 0, optItem);
	while ((dhcpInfo.OptionsCounter < DHCP_OPTIONS_LEN))
	{
		pDecline->Options[dhcpInfo.OptionsCounter ++] = 0;
	}

	DHCP_TimeSet(3);
	DHCP_TimeStart();
}

#endif

/*
 *--------------------------------------------------------------------------------
 * void dhcp_ReceiveMessage(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
U8_T dhcp_ReceiveMessage(U16_T length)
{
	DHCP_HEADER		*pDhcpMsg;
	U8_T			*optMsg, dhcpMsgType = 0;
	U8_T			i, rcvChAddr[6], chkMac = 1;

	pDhcpMsg = (DHCP_HEADER *)dhcpInfo.dhcpPktPtr;

    /* calculate the option length */
    length = length + DHCP_OPTIONS_LEN - sizeof(DHCP_HEADER);

	dhcpInfo.pOpt = &(pDhcpMsg->Options[0]);
	dhcpInfo.OptionsCounter = 0;
	for (i=0 ; i<MAC_ADDRESS_LEN ; i++)
		rcvChAddr[i] = pDhcpMsg->ChAddr[i];

	if (pDhcpMsg->Op == DHCP_S_TO_C)
	{
		/* check the received MAC address within ChAddr, except 00-00-00-00-00-00 & FF-FF-FF-FF-FF-FF */
		if ((rcvChAddr[0]==0 && rcvChAddr[1]==0 && rcvChAddr[2]==0 && rcvChAddr[3]==0 && rcvChAddr[4]==0 && rcvChAddr[5]==0)
		|| (rcvChAddr[0]==0xFF && rcvChAddr[1]==0xFF && rcvChAddr[2]==0xFF && rcvChAddr[3]==0xFF && rcvChAddr[4]==0xFF && rcvChAddr[5]==0xFF))
		{
			chkMac = 0;
		}

		if (pDhcpMsg->Xid == dhcpXid)
		{
			if (chkMac)
			{
				for (i=0 ; i<MAC_ADDRESS_LEN ; i++)
				{
					if (rcvChAddr[i] != dhcpMacAddr[i])
						return DHCPC_MESSAGE_NOT_FOR_ME;
				}
			}
			if ((*(dhcpInfo.pOpt + 0) == 0x63) && (*(dhcpInfo.pOpt + 1) == 0x82) &&
			(*(dhcpInfo.pOpt + 2) == 0x53) && (*(dhcpInfo.pOpt + 3) == 0x63))
			{
				dhcpInfo.OptionsCounter += 4;
				dhcpInfo.pOpt += dhcpInfo.OptionsCounter;
                length-=4;

				i = 0;
                optMsg = dhcp_GetOption(dhcpInfo.pOpt, DHCP_OPTION_MSG_TYPE, &i, length);
                if (optMsg && i == 1)
                    dhcpMsgType = *optMsg;

                /* End of options */
                optMsg = dhcpInfo.pOpt + length;
				while(*dhcpInfo.pOpt != DHCP_OPTION_END && dhcpInfo.pOpt <= optMsg)
				{
					if (*dhcpInfo.pOpt == DHCP_OPTION_MSG_TYPE)
					{
						dhcpInfo.pOpt += 3;
						if (dhcpMsgType == DHCP_ACK)
						{
							if (dhcpInfo.State == DHCP_REQUEST_STATE)
							{
								dhcpInfo.IpAddr = (pDhcpMsg->YiAddr);
								/* Set local IP address */
								/* set uip host ip address */
								TCPIP_SetIPAddr(dhcpInfo.IpAddr);

								/* set stoe host ip address */
								STOE_SetIPAddr((dhcpInfo.IpAddr));
							}
							else if (dhcpInfo.State == DHCP_RENEW_STATE)
							{
								if (pDhcpMsg->YiAddr != STOE_GetIPAddr())
								{
									return DHCPC_MESSAGE_FAIL;
								}
							}
							else
							{
								return DHCPC_MESSAGE_NOT_FOR_ME;
							}
						}
						else if (dhcpMsgType == DHCP_OFFER)
						{
							if (dhcpInfo.State == DHCP_DISCOVER_STATE)
							{
								if (pDhcpMsg->YiAddr)
									*(U32_T*)dhcpInfo.ClientOfferedIp = pDhcpMsg->YiAddr;
							}
							else
							{
								return DHCPC_MESSAGE_NOT_FOR_ME;
							}
						}
						else if (dhcpMsgType == DHCP_NAK)
						{
							return DHCPC_MESSAGE_FAIL;
						}
					}
					else
					{	
						if (dhcpMsgType == DHCP_ACK)
                        {
							if (dhcpInfo.State == DHCP_RENEW_STATE)
							{
								if (pDhcpMsg->YiAddr != STOE_GetIPAddr())
								{
                                    /* The IP is not device requested, keep the lease time only. */
                                    dhcp_OptionCheck(DHCP_NAK);
				                    dhcp_LeaseTime = dhcpInfo.LeaseTime/2;
				                    dhcp_LeaseTimerT1 = dhcp_LeaseTime;
                                    return DHCPC_MESSAGE_SUCCESS;
								}
							}
							dhcp_OptionCheck(DHCP_ACK);
                        }
						else
							dhcp_OptionCheck(DHCP_OFFER);
					}
				}
				return DHCPC_MESSAGE_SUCCESS;
			}
			else
				return DHCPC_MESSAGE_FAIL;
		}
		else
			return DHCPC_MESSAGE_NOT_FOR_ME;
	}

	return DHCPC_MESSAGE_NOT_FOR_ME;
}

/*
 *--------------------------------------------------------------------------------
 * void dhcp_OptionCheck()
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void dhcp_OptionCheck(U8_T msgType)
{
	U8_T	len;

	switch(*(dhcpInfo.pOpt))
	{
		case DHCP_OPTION_SUBNET_MASK:
			memcpy(&(dhcpInfo.NetMask), (dhcpInfo.pOpt + 2), 4);
			len = *(dhcpInfo.pOpt + 1) + 2;
			dhcpInfo.pOpt += len;

			if (msgType == DHCP_ACK)
			{
				/* Set SubNet Mask address */
				/* set uip subnet mask address */
				TCPIP_SetSubnetMask(dhcpInfo.NetMask);

				/* set stoe subnet mask address */
				STOE_SetSubnetMask((dhcpInfo.NetMask));
			}
			break;
		case DHCP_OPTION_ROUTER:
			memcpy(&(dhcpInfo.DefaultRouter), (dhcpInfo.pOpt + 2), 4);
			len = *(dhcpInfo.pOpt + 1) + 2;
			dhcpInfo.pOpt += len;

			if (msgType == DHCP_ACK)
			{
				/* Set Gateway address */
				/* set uip gateway address */
				TCPIP_SetGateway(dhcpInfo.DefaultRouter);

				/* set stoe gateway address */
				STOE_SetGateway((dhcpInfo.DefaultRouter));
			}
			break;
		case DHCP_OPTION_DNS_SERVER:
			memcpy(&(dhcpInfo.DnsAddr), (dhcpInfo.pOpt + 2), 4);
			len = *(dhcpInfo.pOpt + 1) + 2;
			dhcpInfo.pOpt += len;
			break;
		case DHCP_OPTION_SERVER_ID:
			memcpy(&(dhcpInfo.ServerId[0]), (dhcpInfo.pOpt + 2), 4);
			len = *(dhcpInfo.pOpt + 1) + 2;
			dhcpInfo.pOpt += len;
			break;
		case DHCP_OPTION_LEASE_TIME:
			memcpy(&(dhcpInfo.LeaseTime), (dhcpInfo.pOpt + 2), 4);
			len = *(dhcpInfo.pOpt + 1) + 2;
			dhcpInfo.pOpt += len;
			if (msgType == DHCP_ACK)
			{
				dhcp_LeaseTime = dhcpInfo.LeaseTime/2;
				dhcp_LeaseTimerT1 = dhcp_LeaseTime;
			}
            break;
		default :
			len = *(dhcpInfo.pOpt + 1);
			dhcpInfo.pOpt += (len + 2);
			break;
	}
}

/*
 *--------------------------------------------------------------------------------
 * void dhcp_GetOption()
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
U8_T *dhcp_GetOption(U8_T *optMsg, U8_T msgCode, U8_T *vLen, U16_T maxLen)
{
    U8_T len, *optMsgEnd;
    
    optMsgEnd = optMsg + maxLen;
    while(*optMsg != DHCP_OPTION_END && optMsg <= optMsgEnd) /* first to parse the msg type */
    {
        if (*optMsg == msgCode)
        {
            *vLen = *(optMsg + 1);
            optMsg += 2;
            return optMsg;
        }
        else
        {
            len = *(optMsg + 1);
            optMsg += (len + 2);
        }
    }

    return 0;
} /* End of dhcp_GetOption() */

/*
 *--------------------------------------------------------------------------------
 * void dhcp_OptMagicCookie(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void dhcp_OptMagicCookie(void)
{
	*(dhcpInfo.pOpt + 0) = 0x63;
	*(dhcpInfo.pOpt + 1) = 0x82;
	*(dhcpInfo.pOpt + 2) = 0x53;
	*(dhcpInfo.pOpt + 3) = 0x63;
	dhcpInfo.OptionsCounter += 4;
	dhcpInfo.pOpt += dhcpInfo.OptionsCounter;
}

/*
 *--------------------------------------------------------------------------------
 * void dhcp_OptionsAppend(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void dhcp_OptionsAppend(U8_T optCode, U8_T optLen, U8_T *optContent)
{
	U16_T	i;
	U16_T	count = 0;

	*(dhcpInfo.pOpt + 0) = optCode;
	count ++;

	*(dhcpInfo.pOpt + 1) = optLen;
	count ++;

	for (i=0 ; i<optLen ; i++)
	{
		*(dhcpInfo.pOpt + 2 + i) = *(optContent + i);
		count ++;
	}
	

	if (optCode != DHCP_OPTION_END)
	{
		dhcpInfo.pOpt = dhcpInfo.pOpt + count;
		dhcpInfo.OptionsCounter = dhcpInfo.OptionsCounter + count;
	}
	else
	{
		*(dhcpInfo.pOpt) = optCode;
		dhcpInfo.pOpt = dhcpInfo.pOpt + count;
		dhcpInfo.OptionsCounter = dhcpInfo.OptionsCounter + count;
	}

} /* End of dhcp_OptionsAppend() */

/*
 *--------------------------------------------------------------------------------
 * void DHCP_RejectOffer(void)
 * Purpose :
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCP_RejectOffer(void)
{
	DHCP_HEADER		*pDecline;
	U8_T			optItem[3] = {0, 0, 0};
	U16_T			i;

	dhcpInfo.SocketId = TCPIP_UdpNew(dhcpInfo.AppId, dhcpConnID, *((U32_T *)&dhcpInfo.ServerId[0]), (U16_T)DHCP_CLIENT_PORT,
					(U16_T)DHCP_SERVER_PORT);
	if (dhcpInfo.SocketId == TCPIP_NO_NEW_CONN)
	{
		return;
	}

	pDecline = &dhcpBuffer;

	pDecline->Op = DHCP_C_TO_S;
	pDecline->Htype = DHCP_HTYPE_ETHERNET;
	pDecline->Hlen = DHCP_HLEN_ETHERNET;
	pDecline->Hops = 0;
	pDecline->Xid = dhcpXid;
	pDecline->Seconds = 0;
	pDecline->Flags = 0;
	pDecline->CiAddr = 0;
	pDecline->YiAddr = 0;
	pDecline->SiAddr = 0;
	pDecline->GiAddr = 0;
	for (i=0 ; i<MAC_ADDRESS_LEN ; i++)
	{
		if (i < MAC_ADDRESS_LEN)
			pDecline->ChAddr[i] = dhcpMacAddr[i];
		else
			pDecline->ChAddr[i] = 0;
	}
	for (i=0 ; i<DHCP_SNAME_LEN ; i++)
	{
		pDecline->Sname[i] = 0;
	}
	for (i=0 ; i<DHCP_FILE_LEN ; i++)
	{
		pDecline->File[i] = 0;
	}

	dhcpInfo.OptionsCounter = 0;

	/* Add magic cookie bytes */
	dhcpInfo.pOpt = &(pDecline->Options);
	dhcp_OptMagicCookie();

	/* Add DHCP_REQUEST option */
	optItem[0] = DHCP_DECLINE;
	dhcp_OptionsAppend(DHCP_OPTION_MSG_TYPE, 1, optItem);

	/* Add DHCP_OPTION_END option */
	optItem[0] = 0;
	dhcp_OptionsAppend(DHCP_OPTION_END, 0, optItem);
	while ((dhcpInfo.OptionsCounter < DHCP_OPTIONS_LEN))
	{
		pDecline->Options[dhcpInfo.OptionsCounter ++] = 0;
	}

	TCPIP_UdpSend((dhcpInfo.SocketId), NULL, 0, (U8_T *)pDecline, (U16_T)(DHCP_FIXED_LEN + dhcpInfo.OptionsCounter));

	TCPIP_UdpClose(dhcpInfo.SocketId);
}

/*
 *--------------------------------------------------------------------------------
 * Function Name: DHCPC_LeaseTimeCheck
 * Purpose : This function is called every second to check the lease time
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCPC_LeaseTimeCheck(void)
{
	if (!dhcp_LeaseTime) /* ip address not from dhcp server */
		return;

	if (dhcpInfo.State == DHCP_RENEW_STATE)
	{
		dhcp_curTime = (U16_T)SWTIMER_Tick();

		if ((dhcp_curTime - dhcp_Time) >= DHCP_REQUEST_TIMEOUT) /* had timeout */
		{
			if (++dhcp_Retry >= DHCP_MAX_RETRY)
			{
				TCPIP_UdpClose(dhcpInfo.SocketId);
				dhcpInfo.State = DHCP_IDLE_STATE;
			}
			else
			{
				dhcp_Request(0);
				dhcpInfo.State = DHCP_RENEW_STATE;
				dhcp_Time = (U16_T)SWTIMER_Tick();
			}
		}

		if (dhcp_LeaseTimerT1 > 1)
			dhcp_LeaseTimerT1--;
	}
	else if (--dhcp_LeaseTimerT1 == 0) /* lease time expired */
	{
		dhcp_LeaseTime /= 2;
		dhcp_LeaseTimerT1 = dhcp_LeaseTime;
		if (dhcp_LeaseTimerT1 < 10) /* < 10 seconds */
		{
            dhcp_LeaseTime = 0;
		}
		else
		{
			dhcpInfo.SocketId = TCPIP_UdpNew(dhcpInfo.AppId, dhcpConnID, *((U32_T *)&dhcpInfo.ServerId[0]),
				                             (U16_T)DHCP_CLIENT_PORT, (U16_T)DHCP_SERVER_PORT);
			if (dhcpInfo.SocketId == TCPIP_NO_NEW_CONN)
			{
				return;
			}

			dhcp_Retry = 0;
			dhcp_Request(0);
			dhcpInfo.State = DHCP_RENEW_STATE;
			dhcp_Time = (U16_T)SWTIMER_Tick();
		}
	}

} /* End of DHCPC_LeaseTimeCheck() */

/*
 *--------------------------------------------------------------------------------
 * Function Name: DHCPC_LeaseTimeReset
 * Purpose : This function is called to reset parameter dhcp_LeaseTime to zero, so
 *			 driver could not enable dhcp client renew function.
 * Params  :
 * Returns :
 * Note    :
 *--------------------------------------------------------------------------------
 */
void DHCPC_LeaseTimeReset(void)
{
	dhcp_LeaseTime = 0;

} /* End of DHCPC_LeaseTimeReset() */


/* End of dhcpc.c */