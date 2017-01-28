/*
 ******************************************************************************
 *     Copyright (c) 2010	ASIX Electronic Corporation      All rights reserved.
 *
 *     This is unpublished proprietary source code of ASIX Electronic Corporation
 *
 *     The copyright notice above does not evidence any actual or intended
 *     publication of such source code.
 ******************************************************************************
 */
 /*============================================================================
 * Module Name: httpdap.c
 * Purpose:
 * Author:
 * Date:
 * Notes:
 * $Log: httpdap.c,v $
 * no message
 *
 *=============================================================================
 */
#define HTTPDAP_DEBUG	1
#define HTTPDAP_ENABLE_AUTHORITY 0
/* INCLUDE FILE DECLARATIONS */
#include "httpdap.h"
#include "httpdapi.h"
#include "filesys.h"
#include "ax22000.h"

#include "mcpu.h"
#include "printd.h"
#include <string.h>
#include "delay.h"
#include "main.h"
#include "gs2w.h"
#include "gconfig.h"
#if (INCLUDE_EVENT_DETECT)
#include "gevent.h"
#endif

#include "gudpdat.h"
#include "gtcpdat.h"
#include "stoe.h"

#include "mwioctl.h"
#include "mwifapp.h"
#include "hsuart2.h"
#include "mstimer.h"
/* NAMING CONSTANT DECLARATIONS */
#define HTTPDAP_REBOOT_CMD		0x01
#define HTTPDAP_RESTORE_CMD		0x02
#define HTTPDAP_BLDR_FWUPGRADE_CMD	0x03
#define HTTPDAP_FWUPGRADE_CMD	0x04
#define HTTPDAP_WCPU_FWUPGRADE_CMD	0x05

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */
static U8_T *pHttpap_buf;
static U8_T httpap_tmp8;
static U16_T httpap_tmp16;
static U32_T httpap_tmp32;

static U16_T httpap_AuthenTimerEvent = 0;

HTTPDAP_INFO httpdap;
/* NAMING CONSTANT DECLARATIONS */

/* GLOBAL VARIABLES DECLARATIONS */

/* LOCAL VARIABLES DECLARATIONS */

/* LOCAL SUBPROGRAM DECLARATIONS */
/* Add function declaration to support customers' application, start */
U8_T httpap_GetElementValue(U8_T eid, U8_T **pVal);
U8_T httpap_SetElementValue(U8_T eid, U8_T *pVal, U8_T ValLen);
U8_T httpap_CheckElementUpdate(U8_T eid);
U8_T httpap_CheckElementUpdateByName(U8_T *pEname);

U8_T httpap_SecurityProcess(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);

U8_T httpap_GetIndexPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostIndexPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_GetBasicPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostBasicPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_GetAdvancePage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostAdvancePage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_GetSecurityPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostSecurityPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_GetWiFiPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostWiFiPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_GetWiFiWizardPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostWiFiWizardPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
U8_T httpap_PostHctentPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo);
/* Add function declaration to support customers' application, end */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_Init()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HTTPAP_Init(HTTP_SERVER_INFO *pInfo)
{
	pInfo->HomePage = FILE_INDEX;

	/* Add to support customers' application, start */
	memset((U8_T *)&httpdap, 0, sizeof(HTTPDAP_INFO));
	httpdap.AccIPOnOff = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_ACC_IP_ENABLE) ? 1:0;
	
#if HTTPDAP_ENABLE_AUTHORITY	
	pInfo->HomePage = FILE_BASIC;
#endif
	/* Add to support customers' application, end */

} /* End of HTTPAP_Init() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_ResponseComplete()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T HTTPAP_ResponseComplete(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */
	if (pConn->FileId == FILE_SYSMSG)
	{
		switch (httpdap.Cmd)
		{
		case HTTPDAP_REBOOT_CMD:
#if HTTPDAP_DEBUG			
			printd("HTTPDAP: Device Reboot!!\n\r");
#endif			
			DELAY_Ms(500);
			MCPU_SoftReboot();
		break;
		
		case HTTPDAP_RESTORE_CMD:
#if HTTPDAP_DEBUG			
			printd("HTTPDAP: Restore and Reboot!!\n\r");
#endif
			GCONFIG_ReadDefaultConfigData();
			GCONFIG_WriteConfigData();
			DELAY_Ms(500);
			MCPU_SoftReboot();
		break;	
		
		case HTTPDAP_BLDR_FWUPGRADE_CMD:
#if HTTPDAP_DEBUG			
			printd("HTTPDAP: Upgrade Bootloader\n\r");
#endif			
			GCONFIG_EnableFirmwareUpgrade(GCONFIG_FTYPE_BLDR);
			GCONFIG_WriteConfigData();
			MCPU_SoftReboot();
		break;
		
		case HTTPDAP_FWUPGRADE_CMD:
#if HTTPDAP_DEBUG			
			printd("HTTPDAP: Upgrade MCPU\n\r");
#endif			
			GCONFIG_EnableFirmwareUpgrade(GCONFIG_FTYPE_MCPU);
			GCONFIG_WriteConfigData();		
			MCPU_SoftReboot();
		break;
		
		case HTTPDAP_WCPU_FWUPGRADE_CMD:
#if HTTPDAP_DEBUG			
			printd("HTTPDAP: Upgrade WCPU\n\r");
#endif			
			GCONFIG_EnableFirmwareUpgrade(GCONFIG_FTYPE_WCPU);
			GCONFIG_WriteConfigData();		
			MCPU_SoftReboot();
		break;				
		default:
		break;
		}
		httpdap.Cmd = 0;	
	}
	
	/* Add to support customers' application, end */
	return HTTP_POST_OK;
} /* End of HTTPAP_ResponseComplete() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_DivideHtmlFile()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HTTPAP_DivideHtmlFile(HTTP_SERVER_CONN *pConn, U8_T fileId)
{
	/* Add to support customers' application, start */

	/* Add to support customers' application, end */

} /* End of HTTPAP_DivideHtmlFile() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_SendMoreWebFragment()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HTTPAP_SendMoreWebFragment(U8_T index, HTTP_SERVER_CONN *pConn)
{
	/* Add to support customers' application, start */

	/* Add to support customers' application, end */

} /* End of HTTPAP_SendMoreWebFragment() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_GetMethodProcess()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T HTTPAP_GetMethodProcess(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */
	if (httpap_SecurityProcess(pConn, pInfo) == HTTP_POST_ERR)
		return HTTP_POST_ERR;
			
	switch (pConn->FileId)
	{
	case FILE_INDEX:
		return (httpap_GetIndexPage(pConn, pInfo));
		break;
	case FILE_BASIC:
		return (httpap_GetBasicPage(pConn, pInfo));
		break;		
	case FILE_ADVANCED:
		return (httpap_GetAdvancePage(pConn, pInfo));	
		break;		
	case FILE_SECURITY:
		return (httpap_GetSecurityPage(pConn, pInfo));
		break;		
	case FILE_WIFI:			
		return (httpap_GetWiFiPage(pConn, pInfo));	
		break;
	case FILE_WIFIWZ_ADV:
	case FILE_WIFIWZ_STAT:
	case FILE_WIFIWZ_SYS:
	case FILE_WIFIWZ_WEP:
	case FILE_WIFIWZ_WPA:
		return (httpap_GetWiFiWizardPage(pConn, pInfo));		
		break;		
//	case FILE_STATUS:		
//		return (httpap_GetStatusPage(pConn, pInfo));	
//		break;			

	default:
		break;	
	}
	/* Add to support customers' application, end */
	return HTTP_POST_OK;
} /* End of HTTPAP_GetMethodProcess() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_PostMethodPreParseProcess()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T HTTPAP_PostMethodPreParseProcess(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */

	/* Add to support customers' application, end */
	return HTTP_POST_OK;
} /* HTTPAP_PostMethodPreParseProcess() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_PostMethodDeferParseProcess()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T HTTPAP_PostMethodDeferParseProcess(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */

	/* Add to support customers' application, end */
	return HTTP_POST_OK;
} /* HTTPAP_PostMethodDeferParseProcess() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_PostMethodPreUpdateProcess()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T HTTPAP_PostMethodPreUpdateProcess(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */

	/* Add to support customers' application, end */
	return HTTP_POST_OK;
} /* End of HTTPAP_PostMethodPreUpdateProcess() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_PostMethodDeferProcess()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T HTTPAP_PostMethodDeferProcess(U8_T postSuccessFlag, HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */
	if (postSuccessFlag == HTTP_POST_FAILURE)
		return HTTP_POST_ERR;
	
	if (httpap_SecurityProcess(pConn, pInfo) == HTTP_POST_ERR)
		return HTTP_POST_ERR;
			
	switch (pConn->FileId)
	{
	case FILE_INDEX:
		return (httpap_PostIndexPage(pConn, pInfo));
		break;
	case FILE_BASIC:
		return (httpap_PostBasicPage(pConn, pInfo));
		break;		
	case FILE_ADVANCED:
		return (httpap_PostAdvancePage(pConn, pInfo));	
		break;		
	case FILE_SECURITY:
		return (httpap_PostSecurityPage(pConn, pInfo));
		break;		
	case FILE_WIFI:
		return (httpap_PostWiFiPage(pConn, pInfo));	
		break;
	case FILE_WIFIWZ_ADV:
	case FILE_WIFIWZ_ECRYTY:
	case FILE_WIFIWZ_SYS:
	case FILE_WIFIWZ_WEP:
	case FILE_WIFIWZ_WPA:	
	case FILE_WIFIWZ_STAT:	
	case FILE_WIFI_WZ:
		return (httpap_PostWiFiWizardPage(pConn, pInfo));		
		break;
	case FILE_HCTENT:				
		return (httpap_PostHctentPage(pConn, pInfo));	
		break;					
	default:
		break;	
	}
	/* Add to support customers' application, end */
	return HTTP_POST_ERR;

} /* End of HTTPAP_PostMethodDeferProcess() */

/*===========================================================================*/
/*============== Functions Supplied by Customers' Application ===============*/
/*===========================================================================*/
/* Add to support customers' application, start */

/*
 * ----------------------------------------------------------------------------
 * Function Name: HTTPAP_Task
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void HTTPAP_Task(void)
{
	U8_T i;

	if (httpap_AuthenTimerEvent != (U16_T)SWTIMER_Tick())
	{
		httpap_AuthenTimerEvent = (U16_T)SWTIMER_Tick();	
		for (i = 0; i < MAX_HTTP_CONNECT; i++)
		{
			if (httpdap.SecurityTimeExpired[i])
				httpdap.SecurityTimeExpired[i] --;
		}
	}
} /* End of HTTPAP_Task() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetElementValue()
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetElementValue(U8_T eid, U8_T **pVal)
{
	if (eid >= MAX_POST_RECORDS)
		return 0;

	if (POST_Record[eid].PostType == POST_TYPE_TEXT || 
		POST_Record[eid].PostType == POST_TYPE_TAG || 
		POST_Record[eid].PostType == POST_TYPE_PASSWORD)
	{
		BUF_TEXT XDATA *pText = POST_Record[eid].PValue;	
		*pVal = pText->CurrValue;
		return (pText->CurrLength - 1);
	}
	else if (POST_Record[eid].PostType == POST_TYPE_SELECT)
	{
		BUF_SELECT XDATA *pSelect = POST_Record[eid].PValue;
		*pVal = &(pSelect->CurrentSet);
		return 1;
	}
#if (MAX_POST_BUF_RADIO)
	else if (POST_Record[eid].PostType == POST_TYPE_RADIO)
	{
		BUF_RADIO XDATA *pRadio = POST_Record[eid].PValue;
		*pVal = &(pRadio->CurrentSet);
		return 1;
	}
#endif		
	else if (POST_Record[eid].PostType == POST_TYPE_SUBMIT)
	{
		BUF_SUBMIT XDATA *pSubmit = POST_Record[eid].PValue;	
		*pVal = pSubmit->Value;
		return (pSubmit->DefaultVlaueLen);
	}			
	return 0; /* element type not support */
		
} /* End of httpap_GetElementValue() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_SetElementValue()
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_SetElementValue(U8_T eid, U8_T *pVal, U8_T ValLen)
{
	if (eid >= MAX_POST_RECORDS)
		return HTTPAPI_ERR;
	
	if (POST_Record[eid].PostType == POST_TYPE_TEXT || 
		POST_Record[eid].PostType == POST_TYPE_TAG || 
		POST_Record[eid].PostType == POST_TYPE_PASSWORD)
	{
		BUF_TEXT XDATA *pText = POST_Record[eid].PValue;	
		memcpy(pText->CurrValue, pVal, ValLen);		
		pText->CurrValue[ValLen] = '"';
		pText->CurrLength = ValLen + 1;		
		return POST_Record[eid].PostType;
	}
	else if (POST_Record[eid].PostType == POST_TYPE_SELECT)
	{
		BUF_SELECT XDATA *pSelect = POST_Record[eid].PValue;		
		if (*pVal < pSelect->Count)
			pSelect->CurrentSet = *pVal;		
		else
			pSelect->CurrentSet = pSelect->Count-1;

		return POST_Record[eid].PostType;
	}
#if (MAX_POST_BUF_RADIO)
	else if (POST_Record[eid].PostType == POST_TYPE_RADIO)
	{
		BUF_RADIO XDATA *pRadio = POST_Record[eid].PValue;
		if (*pVal < pRadio->Count)
			pRadio->CurrentSet = *pVal;		
		else
			pRadio->CurrentSet = pRadio->Count-1;
		return POST_Record[eid].PostType;
	}
#endif
	return HTTPAPI_ERR; /* element type not support */
} /* End of httpap_SetElementValue() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: U8_T httpap_CheckElementUpdate()
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_CheckElementUpdate(U8_T eid)
{
	U8_T i;
	
	if (HTTP_PostCount == 0)
		return HTTPAPI_ERR;
		
	for (i = 0; i < HTTP_PostCount; i++)
	{
		if (HTTP_PostTable[i] == eid)
			return i;
	}
	return HTTPAPI_ERR;
} /* End of httpap_CheckElementUpdate() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: U8_T httpap_CheckElementUpdateByName()
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_CheckElementUpdateByName(U8_T *pEname)
{
	U8_T i;
	
	if (HTTP_PostCount == 0)
		return HTTPAPI_ERR;
		
	for (i = 0; i < HTTP_PostCount; i++)
	{
		if ((strlen(pEname) == POST_Record[HTTP_PostTable[i]].NameLen) && 
			memcmp(pEname, POST_Record[HTTP_PostTable[i]].Name, POST_Record[HTTP_PostTable[i]].NameLen) == 0)
			return i;
	}
	return HTTPAPI_ERR;
} /* End of httpap_CheckElementUpdateByName() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_SecurityProcess()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_SecurityProcess(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Add to support customers' application, start */
	U8_T i;
#if HTTPDAP_ENABLE_AUTHORITY	
	U8_T *pUsername, *pPassword, *pbuf1, *pbuf2;
#endif

		
#if HTTPDAP_ENABLE_AUTHORITY
	//===================== RFC 2617 support ======================= //
	if (HTTPAPI_ParseAuthenHeader(HTTP_TmpBuf, pInfo->pRequestBuf, &pUsername, &pPassword) == HTTPAPI_ERR)
	{
#if HTTPDAP_DEBUG		
		printd("HTTPDAP: Authenication header not found\r\n");		
#endif
HTTPDAP_AUTHORITY_REQUST:	
#if (INCLUDE_EVENT_DETECT)
		GEVENT_SetAuthFailEvent(1);// Add for authentication fail detect //
#endif
		HTTPAPI_ResponseAuthenMessage(HTTP_TmpBuf, "Please enter username and password", pConn);
		return HTTP_POST_ERR;
	}
	else
#if HTTPDAP_DEBUG
		printd("HTTPDAP: header is found, user name is %s, password is %s\r\n", pUsername, pPassword);		
#endif	
	// get uername and password for reference //
	pbuf1 = HTTP_TmpBuf+100;
	pbuf2 = HTTP_TmpBuf+150;	
	GCONFIG_GetAdmin(pbuf1, pbuf2, &i);
	
	// check username //
	if (strlen(pUsername) != strlen(pbuf1) || strcmp(pUsername,pbuf1))
	{
		goto HTTPDAP_AUTHORITY_REQUST;
	}
	// check password //
	if (strlen(pPassword) != strlen(pbuf2) || strcmp(pPassword,pbuf2))
	{
		goto HTTPDAP_AUTHORITY_REQUST;
	}
	
	httpap_tmp16 = 0xffff;
	for (i = 0; i < MAX_HTTP_CONNECT; i++)
	{
		// found the empty security slot //
		if (httpdap.SecurityTimeExpired[i] == 0)
		{
			httpdap.SecurityTimeExpired[i] = 0xffff;
			httpdap.SecurityRemoteIP[i] = pConn->Ip;			
			break;
		}
		
		// found a slot is not most commonly used //
		if (httpap_tmp16 > httpdap.SecurityTimeExpired[i])
		{
			httpap_tmp16 = httpdap.SecurityTimeExpired[i];
			httpap_tmp8 = i;
		}
	}
	if (i == MAX_HTTP_CONNECT)
	{
		httpdap.SecurityTimeExpired[httpap_tmp8] = 0xffff;
		httpdap.SecurityRemoteIP[httpap_tmp8] = pConn->Ip;
	}
#else 
	//==================== Web page authentication support =======================//
	if (pConn->FileId != pInfo->HomePage)
	{
		for (i = 0; i < MAX_HTTP_CONNECT; i++)
		{
			if (httpdap.SecurityRemoteIP[i] == pConn->Ip)
				break;
		}
		
		if (i == MAX_HTTP_CONNECT || httpdap.SecurityTimeExpired[i] == 0)
		{
			HTTPAPI_ResponseErrMessage(HTTP_TmpBuf, HTTPAPI_406_NotAccepTable, pConn);	
			return HTTP_POST_ERR;	
		}
		
		httpdap.SecurityTimeExpired[i] = 0xffff;// fill a new security time-expired //
	}
#endif
	/* Add to support customers' application, end */
	return HTTP_POST_OK;
} /* httpap_SecurityProcess() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetIndexPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetIndexPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
#if HTTPDAP_ENABLE_AUTHORITY
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"Logout");
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;/* EXTERNAL DROP */
#else
	U8_T i;
	
	for (i=0; i < MAX_HTTP_CONNECT; i++)
	{
		if (httpdap.SecurityRemoteIP[i] == pConn->Ip)
		{
			httpdap.SecurityTimeExpired[i] = 0;		
			break;
		}
	}

	httpap_SetElementValue(RECORD_TEXT_username, 0, 0);
	httpap_SetElementValue(RECORD_PASSWORD_password, 0, 0);
#endif	
	return HTTP_POST_OK;
} /* End of httpap_GetIndexPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostIndexPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostIndexPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T i, j;
	U8_T *pTmp, XDATA *pbuf1, XDATA *pbuf2;

	if (httpap_CheckElementUpdate(RECORD_TEXT_username) != HTTPAPI_ERR &&
		httpap_CheckElementUpdate(RECORD_PASSWORD_password) != HTTPAPI_ERR)
	{
		// get uername and password for reference //
		pbuf1 = HTTP_TmpBuf;
		pbuf2 = HTTP_TmpBuf + 100;		
		GCONFIG_GetAdmin(pbuf1, pbuf2, &i);
			
		// check username //
		i = httpap_GetElementValue(RECORD_TEXT_username, &pTmp);
		j = strlen(pbuf1);
		if (i != j || memcmp(pbuf1, pTmp, j))
		{
			goto HTTPAP_INDEX_POST_ERR;
		}
			
		// check password //
		i =	httpap_GetElementValue(RECORD_PASSWORD_password, &pTmp);
		j = strlen(pbuf2);
		if (i != j || memcmp(pbuf2, pTmp, j))
		{
			goto HTTPAP_INDEX_POST_ERR;
		}
		httpap_tmp16 = 0xffff;
		for (i = 0; i < MAX_HTTP_CONNECT; i++)
		{
			// found the empty security slot //
			if (httpdap.SecurityTimeExpired[i] == 0)
			{
				httpdap.SecurityTimeExpired[i] = 0xffff;
				httpdap.SecurityRemoteIP[i] = pConn->Ip;			
				break;
			}
		
			// found a slot is not most commonly used //
			if (httpap_tmp16 > httpdap.SecurityTimeExpired[i])
			{
				httpap_tmp16 = httpdap.SecurityTimeExpired[i];
				httpap_tmp8 = i;
			}
		}
		if (i == MAX_HTTP_CONNECT)
		{
			httpdap.SecurityTimeExpired[httpap_tmp8] = 0xffff;
			httpdap.SecurityRemoteIP[httpap_tmp8] = pConn->Ip;			
		}
		goto HTTPAP_INDEX_POST_OK;
	}

HTTPAP_INDEX_POST_ERR:
#if (INCLUDE_EVENT_DETECT)
	GEVENT_SetAuthFailEvent(1);// Add for authentication fail detect //
#endif	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Username or Password error",2,"sec","off",FSYS_Manage[pConn->FileId].FName);
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;
		
HTTPAP_INDEX_POST_OK:	
	HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_BASIC, pConn);// re-direction to basic page //
	return HTTP_POST_ERR;
			
} /* End of httpap_PostIndexPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetBasicPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetBasicPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Load device name */
	pHttpap_buf = (U8_T *)(GCONFIG_GetDeviceName());
	httpap_tmp8 = strlen(pHttpap_buf);
	if (httpap_tmp8 > 16)
		httpap_tmp8 = 16;
	httpap_SetElementValue(RECORD_TEXT_dsm_name, pHttpap_buf, httpap_tmp8);	
	
	/* Load serial setting */
	httpap_tmp16 = GCONFIG_GetSerialPort();

	httpap_tmp8 = ((httpap_tmp16 & 0xF800) >> 11);
	httpap_SetElementValue(RECORD_SELECT_dbr, &httpap_tmp8, 1);

	httpap_tmp8 = ((httpap_tmp16 & 0x0700) >> 8);
	httpap_SetElementValue(RECORD_SELECT_data, &httpap_tmp8, 1);

	httpap_tmp8 = ((httpap_tmp16 & 0x00E0) >> 5);
	httpap_SetElementValue(RECORD_SELECT_parity, &httpap_tmp8, 1);

	httpap_tmp8 = ((httpap_tmp16 & 0x0003));
	httpap_SetElementValue(RECORD_SELECT_stop, &httpap_tmp8, 1);

	httpap_tmp8 = ((httpap_tmp16 & 0x0001C) >> 2);
	httpap_SetElementValue(RECORD_SELECT_flow, &httpap_tmp8, 1);

	/* Load DHCP/Connection type/Connection mode/Accessiable control */
	httpap_tmp16 = GCONFIG_GetNetwork();
	
	httpap_tmp8 = (httpap_tmp16 & GCONFIG_NETWORK_DHCP_ENABLE) ? 1 : 0;
	httpap_SetElementValue(RECORD_SELECT_dhcp, &httpap_tmp8, 1);
	
	httpap_tmp8 = (httpap_tmp16 & GCONFIG_NETWORK_PROTO_TCP) ? 0 : 1;
	httpap_SetElementValue(RECORD_SELECT_conntype, &httpap_tmp8, 1);
		
	httpap_tmp8 = (httpap_tmp16 & GCONFIG_NETWORK_CLIENT) ? 1 : 0;
	httpap_SetElementValue(RECORD_SELECT_cs_mode, &httpap_tmp8, 1);

	/* Load IP address */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetServerStaticIP(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_static_ip, HTTP_TmpBuf, httpap_tmp8);
		
	/* Load subnet mask */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetNetmask(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_mask, HTTP_TmpBuf, httpap_tmp8);
		
	/* Load gateway */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetGateway(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_gateway_ip, HTTP_TmpBuf, httpap_tmp8);

	/* Load DNS */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetDNS(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_dns_ip, HTTP_TmpBuf, httpap_tmp8);

	/* Load server listen port */
	httpap_tmp8 = HTTPAPI_Short2Str(GCONFIG_GetServerDataPktListenPort(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_s_lstport, HTTP_TmpBuf, httpap_tmp8);

	/* Load transmit timer */
	httpap_tmp8 = HTTPAPI_Short2Str(GCONFIG_GetEthernetTxTimer(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_txtimer, HTTP_TmpBuf, httpap_tmp8);

	/* Load destination port */
	httpap_tmp8 = HTTPAPI_Short2Str(GCONFIG_GetClientDestPort(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_c_desport, HTTP_TmpBuf, httpap_tmp8);

	/* Load Destination host name */
	httpap_tmp8 = GCONFIG_GetDestHostName(HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_c_deshn, HTTP_TmpBuf, httpap_tmp8);
	
	/* Load rs485 control bits */
	httpap_tmp8 = GCONFIG_GetRs485Mode();
	httpap_SetElementValue(RECORD_SELECT_rs485, &httpap_tmp8, 1);
		
	return HTTP_POST_OK;
} /* End of httpap_GetBasicPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostBasicPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostBasicPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	
	/* Device reboot process */
	if (httpap_CheckElementUpdateByName("reboot") != HTTPAPI_ERR)
	{
		goto HTTPAP_POST_BASIC_OK_REBOOT;		
	}
	/* Device restore process */	
	else if (httpap_CheckElementUpdateByName("restore") != HTTPAPI_ERR)
	{
		goto HTTPAP_POST_BASIC_OK_RESTORE;		
	}
	else if (httpap_CheckElementUpdate(RECORD_TEXT_dsm_name) != HTTPAPI_ERR && 
		httpap_CheckElementUpdate(RECORD_TEXT_c_desport) != HTTPAPI_ERR)
	{
		// Save device name //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_dsm_name, &pHttpap_buf);
		memcpy(HTTP_TmpBuf,pHttpap_buf,httpap_tmp8);
		memset(HTTP_TmpBuf+httpap_tmp8, 0 , 16-httpap_tmp8);// To fill the unused data with terminal char('\0').		
		GCONFIG_SetDeviceName((GCONFIG_DEV_NAME*)HTTP_TmpBuf);
			
		// Save serial setting //
		httpap_tmp16 = GCONFIG_GetSerialPort();
		httpap_GetElementValue(RECORD_SELECT_dbr, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & (~0xF800)) | (*pHttpap_buf << 11);
	
		httpap_GetElementValue(RECORD_SELECT_data, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & (~0x0700)) | (*pHttpap_buf << 8);

		httpap_GetElementValue(RECORD_SELECT_parity, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & (~0x00E0)) | (*pHttpap_buf << 5);
	
		httpap_GetElementValue(RECORD_SELECT_stop, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & (~0x0003)) | *pHttpap_buf;

		httpap_GetElementValue(RECORD_SELECT_flow, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & (~0x001C)) | (*pHttpap_buf << 2);
		GCONFIG_SetSerialPort(httpap_tmp16);
	
		// Save DHCP/Connection type/Connection mode //
		httpap_tmp16 = GCONFIG_GetNetwork();
		httpap_GetElementValue(RECORD_SELECT_dhcp, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & 0xBFFF) | (*pHttpap_buf << 14);
	
		httpap_GetElementValue(RECORD_SELECT_conntype, &pHttpap_buf);
		httpap_tmp16 = httpap_tmp16 & 0xF3FF;
		if (*pHttpap_buf)	
			httpap_tmp16 |= 0x0800;//UDP mode
		else		
			httpap_tmp16 |= 0x0400;//TCP mode
			
		httpap_GetElementValue(RECORD_SELECT_cs_mode, &pHttpap_buf);
		httpap_tmp16 = (httpap_tmp16 & 0x7FFF) | (*pHttpap_buf << 15);
		GCONFIG_SetNetwork(httpap_tmp16);

		// Save IP address //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_static_ip, &pHttpap_buf);
		GCONFIG_SetServerStaticIP(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));

		// Save subnet mask //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_mask, &pHttpap_buf);
		GCONFIG_SetNetmask(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));
		
		// Save gateway //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_gateway_ip, &pHttpap_buf);
		GCONFIG_SetGateway(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));

		// Save DNS //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_dns_ip, &pHttpap_buf);
		GCONFIG_SetDNS(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));

		// Save server listen port //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_s_lstport, &pHttpap_buf);
		GCONFIG_SetServerDataPktListenPort(HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8));

		// Save transmit timer //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_txtimer, &pHttpap_buf);
		GCONFIG_SetEthernetTxTimer(HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8));
	
		// Save destination port //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_c_desport, &pHttpap_buf);
		GCONFIG_SetClientDestPort(HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8));
	
		// Save Destination host name //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_c_deshn, &pHttpap_buf);
		GCONFIG_SetDestHostName(pHttpap_buf, httpap_tmp8);

		// Save rs485 control bits //
		httpap_GetElementValue(RECORD_SELECT_rs485, &pHttpap_buf);
		GCONFIG_SetRs485Mode(*pHttpap_buf);
		
		goto HTTPAP_POST_BASIC_OK_SAVE_AND_REBOOT;
	}
HTTPAP_POST_BASIC_ERR:
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Settings validation error!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;	

HTTPAP_POST_BASIC_OK_SAVE_AND_REBOOT:
	// Store configuration to flash ROM //
	GCONFIG_WriteConfigData();	
HTTPAP_POST_BASIC_OK_REBOOT:
	httpdap.Cmd = HTTPDAP_REBOOT_CMD;
	httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 20 : 15;//select delay time for DHCP enable or disable. //			
		
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
										 ,"Device reboot! please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;	
													
HTTPAP_POST_BASIC_OK_RESTORE:
	httpdap.Cmd = HTTPDAP_RESTORE_CMD;	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);		
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
										// warning message, delay time, delay time unit, delay time visible, redirect URL //
										 ,"Device restore to default! please Wait",20,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);		
	return HTTP_POST_ERR;															
} /* End of httpap_PostBasicPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetAdvancePage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetAdvancePage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Load TFTP IP addr and file name for boot loarder */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetTftpServerIp(GCONFIG_FTYPE_BLDR), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_tftps_ip_b, HTTP_TmpBuf, httpap_tmp8);
	
	GCONFIG_GetFilename(HTTP_TmpBuf, 128, GCONFIG_FTYPE_BLDR);
	httpap_tmp8 = strlen(HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_file_name_b, HTTP_TmpBuf, httpap_tmp8);
		
	/* Load TFTP IP addr and file name for MCPU */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetTftpServerIp(GCONFIG_FTYPE_MCPU), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_tftps_ip, HTTP_TmpBuf, httpap_tmp8);
	
	GCONFIG_GetFilename(HTTP_TmpBuf, 128, GCONFIG_FTYPE_MCPU);
	httpap_tmp8 = strlen(HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_file_name, HTTP_TmpBuf, httpap_tmp8);

	/* Load TFTP IP addr and file name for WCPU */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetTftpServerIp(GCONFIG_FTYPE_WCPU), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_tftps_ip_w, HTTP_TmpBuf, httpap_tmp8);
	
	GCONFIG_GetFilename(HTTP_TmpBuf, 128, GCONFIG_FTYPE_WCPU);
	httpap_tmp8 = strlen(HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_file_name_w, HTTP_TmpBuf, httpap_tmp8);
	


	/* Load DHCP server status */
	httpap_tmp8 = GCONFIG_GetDhcpSrvStatus() ? 1 : 0;
	httpap_SetElementValue(RECORD_RADIO_dhcps_onoff, &httpap_tmp8, 1);
		
	/* Load DHCP server IP pool start address */
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetDhcpSrvStartIp(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_dhcps_startip, HTTP_TmpBuf, httpap_tmp8);
	
	/* Load DHCP server IP pool end address */		
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetDhcpSrvEndIp(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_dhcps_endip, HTTP_TmpBuf, httpap_tmp8);
	
	/* Load DHCP server default gateway */		
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetDhcpSrvDefGateway(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_dhcps_gateway, HTTP_TmpBuf, httpap_tmp8);	
	
	/* Load DHCP server netmask */		
	httpap_tmp8 = HTTPAPI_Ulong2IpAddr(GCONFIG_GetDhcpSrvNetmask(), HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_dhcps_mask, HTTP_TmpBuf, httpap_tmp8);	
	return HTTP_POST_OK;
} /* End of httpap_GetAdvancePage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostAdvancePage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostAdvancePage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T *pTmp, *pbuf1, *pbuf2, tmp;
	
	pbuf1 = HTTP_TmpBuf;
	pbuf2 = HTTP_TmpBuf + 50;
	
	 if (httpap_CheckElementUpdate(RECORD_TEXT_tftps_ip_b) != HTTPAPI_ERR && 
		 httpap_CheckElementUpdate(RECORD_TEXT_file_name_b) != HTTPAPI_ERR)
	{
		// Save TFTP IP addr //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_tftps_ip_b, &pHttpap_buf);
		GCONFIG_SetTftpServerIp(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8), GCONFIG_FTYPE_BLDR);
			
		// Save TFTP File name //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_file_name_b, &pHttpap_buf);
		GCONFIG_SetFilename(pHttpap_buf, httpap_tmp8,GCONFIG_FTYPE_BLDR);

		// process firmware upgrade //
		if (httpap_CheckElementUpdateByName("FwBldUpg") != HTTPAPI_ERR)
		{
			// Store configuration to flash ROM //
//			GCONFIG_WriteConfigData();	

			httpdap.Cmd = HTTPDAP_BLDR_FWUPGRADE_CMD;
			httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 20 : 15;//select delay time for DHCP enable or disable. //				
			
			pHttpap_buf = HTTP_TmpBuf;
			httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
			HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
			return HTTP_POST_ERR;
		}
					
		goto HTTPAP_POST_ADVANCE_OK;
	}	
	else if (httpap_CheckElementUpdate(RECORD_TEXT_tftps_ip) != HTTPAPI_ERR && 
		httpap_CheckElementUpdate(RECORD_TEXT_file_name) != HTTPAPI_ERR)
	{
		// Save TFTP IP addr //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_tftps_ip, &pHttpap_buf);
		GCONFIG_SetTftpServerIp(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8), GCONFIG_FTYPE_MCPU);
			
		// Save TFTP File name //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_file_name, &pHttpap_buf);
		GCONFIG_SetFilename(pHttpap_buf, httpap_tmp8,GCONFIG_FTYPE_MCPU);

		// process firmware upgrade //
		if (httpap_CheckElementUpdateByName("upgrade") != HTTPAPI_ERR)
		{
			// Store configuration to flash ROM //
//			GCONFIG_WriteConfigData();	

			httpdap.Cmd = HTTPDAP_FWUPGRADE_CMD;
			httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 60 : 55;//select delay time for DHCP enable or disable. //				
			
			pHttpap_buf = HTTP_TmpBuf;
			httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
			HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
			return HTTP_POST_ERR;
		}
					
		goto HTTPAP_POST_ADVANCE_OK;
	}
	else if (httpap_CheckElementUpdate(RECORD_TEXT_tftps_ip_w) != HTTPAPI_ERR && 
		     httpap_CheckElementUpdate(RECORD_TEXT_file_name_w) != HTTPAPI_ERR)
	{
		// Save TFTP IP addr //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_tftps_ip_w, &pHttpap_buf);
		GCONFIG_SetTftpServerIp(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8), GCONFIG_FTYPE_WCPU);
			
		// Save TFTP File name //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_file_name_w, &pHttpap_buf);
		GCONFIG_SetFilename(pHttpap_buf, httpap_tmp8,GCONFIG_FTYPE_WCPU);

		// process firmware upgrade //
		if (httpap_CheckElementUpdateByName("FwWUpg") != HTTPAPI_ERR)
		{
			// Store configuration to flash ROM //
//			GCONFIG_WriteConfigData();	

			httpdap.Cmd = HTTPDAP_WCPU_FWUPGRADE_CMD;
			httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 50 : 45;//select delay time for DHCP enable or disable. //				
			
			pHttpap_buf = HTTP_TmpBuf;
			httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
			HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
			return HTTP_POST_ERR;
		}
					
		goto HTTPAP_POST_ADVANCE_OK;
	}				
	else if (httpap_CheckElementUpdate(RECORD_RADIO_dhcps_onoff) != HTTPAPI_ERR &&
			 httpap_CheckElementUpdate(RECORD_TEXT_dhcps_mask) != HTTPAPI_ERR)	
	{
		// Save DHCP server status //
		httpap_GetElementValue(RECORD_RADIO_dhcps_onoff, &pHttpap_buf);
		GCONFIG_SetDhcpSrvStatus(*pHttpap_buf);
		
		// Save DHCP server IP pool start address //		
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_dhcps_startip, &pHttpap_buf);		
		GCONFIG_SetDhcpSrvStartIp(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));

		// Save DHCP server IP pool end address //		
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_dhcps_endip, &pHttpap_buf);				
		GCONFIG_SetDhcpSrvEndIp(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));		
	
		// Save DHCP server default gateway //		
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_dhcps_gateway, &pHttpap_buf);				
		GCONFIG_SetDhcpSrvDefGateway(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));				
	
		// Save DHCP server netmask //		
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_dhcps_mask, &pHttpap_buf);				
		GCONFIG_SetDhcpSrvNetmask(HTTPAPI_IpAddr2Ulong(pHttpap_buf, httpap_tmp8));	
		
		goto HTTPAP_POST_ADVANCE_OK_SAVE_AND_REBOOT;		
	}			
HTTPAP_POST_ADVANCE_ERR:	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Settings validation error!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);	
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;

HTTPAP_POST_ADVANCE_OK:	
	// Store configuration to flash ROM //
	GCONFIG_WriteConfigData();

	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Settings validation ok!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);	
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;
				
HTTPAP_POST_ADVANCE_OK_SAVE_AND_REBOOT:
	// Store configuration to flash ROM //
	GCONFIG_WriteConfigData();	

	httpdap.Cmd = HTTPDAP_REBOOT_CMD;
	httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 20 : 15;//select delay time for DHCP enable or disable. //			
	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);		
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
										// warning message, delay time, delay time unit, delay time visible, redirect URL //
										 ,"Device reboot! please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);		
	return HTTP_POST_ERR;
} /* End of httpap_PostAdvancePage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetSecurityPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetSecurityPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T i, j;
	
	/* Clear new user and password */
	httpap_SetElementValue(RECORD_TEXT_new_usn, 0, 0);
	httpap_SetElementValue(RECORD_PASSWORD_old_psw, 0, 0);
	httpap_SetElementValue(RECORD_PASSWORD_new_psw, 0, 0);
	httpap_SetElementValue(RECORD_PASSWORD_cfm_psw, 0, 0);

	return HTTP_POST_OK;
} /* End of httpap_GetSecurityPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostSecurityPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostSecurityPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T *pTmp, *pbuf1, *pbuf2, tmp;
	
	pbuf1 = HTTP_TmpBuf;
	pbuf2 = HTTP_TmpBuf + 50;
	
	if (httpap_CheckElementUpdate(RECORD_TEXT_new_usn) != HTTPAPI_ERR)	
	{
		// Save new username //
		GCONFIG_GetAdmin(pbuf1, pbuf2, &httpap_tmp8);
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_new_usn, &pHttpap_buf);
		
		memcpy(pbuf1, pHttpap_buf, httpap_tmp8);
		pbuf1[httpap_tmp8] = '\0';
		GCONFIG_SetAdmin(pbuf1, pbuf2);
		
		// Store configuration to flash ROM //
		goto HTTPAP_POST_SECURITY_OK_SAVE_AND_REBOOT;
			
	}
	else if (httpap_CheckElementUpdate(RECORD_PASSWORD_old_psw) != HTTPAPI_ERR && 
			 httpap_CheckElementUpdate(RECORD_PASSWORD_cfm_psw) != HTTPAPI_ERR)
	{
		// check old password //
		GCONFIG_GetAdmin(pbuf1, pbuf2, &httpap_tmp8);
		httpap_tmp8 = httpap_GetElementValue(RECORD_PASSWORD_old_psw, &pHttpap_buf);
		if (strlen(pbuf2) != httpap_tmp8 || memcmp(pbuf2, pHttpap_buf, httpap_tmp8) != 0)
			goto HTTPAP_POST_SECURITY_ERR;
		
		// check new password and confirm password //
		tmp = httpap_GetElementValue(RECORD_PASSWORD_cfm_psw, &pTmp);		
		httpap_tmp8 = httpap_GetElementValue(RECORD_PASSWORD_new_psw, &pHttpap_buf);
		if (tmp != httpap_tmp8 || memcmp(pTmp, pHttpap_buf, httpap_tmp8) != 0)
			goto HTTPAP_POST_SECURITY_ERR;

		// save new password //		
		memcpy(pbuf2, pHttpap_buf, httpap_tmp8);
		pbuf2[httpap_tmp8] = '\0';
		GCONFIG_SetAdmin(pbuf1, pbuf2);

		// Store configuration to flash ROM //
		goto HTTPAP_POST_SECURITY_OK_SAVE_AND_REBOOT;
	}
	
HTTPAP_POST_SECURITY_ERR:	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Settings validation error!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);	
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;
	
	
HTTPAP_POST_SECURITY_OK:	
	// Store configuration to flash ROM //
	GCONFIG_WriteConfigData();

	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Settings validation ok!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);	
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;

HTTPAP_POST_SECURITY_OK_SAVE_AND_REBOOT:	
	// Store configuration to flash ROM //
	GCONFIG_WriteConfigData();

	httpdap.Cmd = HTTPDAP_REBOOT_CMD;
	httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 20 : 15;//select delay time for DHCP enable or disable. //	
	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
										 ,"Device reboot! please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);										
	return HTTP_POST_ERR;
} /* End of httpap_PostSecurityPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetWiFiPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetWiFiPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	/* Network mode */
	httpap_tmp8 = GCONFIG_GetWifiNetworkMode() ? 1:0;
	httpap_SetElementValue(RECORD_SELECT_network_mode, &httpap_tmp8, 1);
	
	/* Channel */
	httpap_tmp8 = GCONFIG_GetChannel();
	httpap_SetElementValue(RECORD_SELECT_channel, &httpap_tmp8, 1);
	
	/* Service Area Name/SSID */
	httpap_tmp8 = GCONFIG_GetSsid(HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_ssid, HTTP_TmpBuf, httpap_tmp8);
			
	/* Secirity Mode */
	httpap_tmp8 = GCONFIG_GetWifiEncryptMode();// % 5;
	httpap_SetElementValue(RECORD_SELECT_security_mode, &httpap_tmp8, 1);
	
	/* Key Length */	
	httpap_tmp8 = GCONFIG_GetWifiWepKeyLength() ? 1:0;
	httpap_SetElementValue(RECORD_SELECT_key_length, &httpap_tmp8, 1);
	
	if (httpap_tmp8)
	{
		/* WEP 128bits Key 0~3 */
		GCONFIG_GetWifiWep128Key(0, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
		httpap_SetElementValue(RECORD_TEXT_key_index_0, HTTP_TmpBuf+100, 26);

		GCONFIG_GetWifiWep128Key(1, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
		httpap_SetElementValue(RECORD_TEXT_key_index_1, HTTP_TmpBuf+100, 26);

		GCONFIG_GetWifiWep128Key(2, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
		httpap_SetElementValue(RECORD_TEXT_key_index_2, HTTP_TmpBuf+100, 26);

		GCONFIG_GetWifiWep128Key(3, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
		httpap_SetElementValue(RECORD_TEXT_key_index_3, HTTP_TmpBuf+100, 26);	
	}
	else
	{
		/* WEP 64bits Key 0~3 */
		GCONFIG_GetWifiWep64Key(0, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
		httpap_SetElementValue(RECORD_TEXT_key_index_0, HTTP_TmpBuf+100, 10);

		GCONFIG_GetWifiWep64Key(1, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
		httpap_SetElementValue(RECORD_TEXT_key_index_1, HTTP_TmpBuf+100, 10);

		GCONFIG_GetWifiWep64Key(2, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
		httpap_SetElementValue(RECORD_TEXT_key_index_2, HTTP_TmpBuf+100, 10);

		GCONFIG_GetWifiWep64Key(3, HTTP_TmpBuf);
		HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
		httpap_SetElementValue(RECORD_TEXT_key_index_3, HTTP_TmpBuf+100, 10);
	}	
	
	/* Key Index Select */
	httpap_tmp8 = GCONFIG_GetWifiWepKeyIndex();// % 4;
	httpap_SetElementValue(RECORD_SELECT_key_index, &httpap_tmp8, 1);

	/* AES/TKIP Passphrase */	
	httpap_tmp8 = GCONFIG_GetWifiPreShareKey(HTTP_TmpBuf);	
	httpap_SetElementValue(RECORD_TEXT_aes_passphrase, HTTP_TmpBuf, httpap_tmp8);
	
	/* TX Data Rate */	
	httpap_tmp8 = GCONFIG_GetWifiTxRate();//%sizof(IO_WiFiSupportTxRate);
	httpap_SetElementValue(RECORD_SELECT_tx_data_rate, &httpap_tmp8, 1);
	
	/* Transmission Power */	
	httpap_tmp8 = GCONFIG_GetWifiTxPowerLevel();
	httpap_SetElementValue(RECORD_SELECT_tx_power, &httpap_tmp8, 1);
	
	/* Preamble */	
	httpap_tmp8 = GCONFIG_GetWifiPreambleMode();
	httpap_SetElementValue(RECORD_SELECT_preamble, &httpap_tmp8, 1);
	
	/* Beacon Interval */	
	httpap_tmp16 = GCONFIG_GetWifiBcnInterval();
	httpap_tmp8 = HTTPAPI_Short2Str(httpap_tmp16, HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_beacon_intv, HTTP_TmpBuf, httpap_tmp8);
	
	/* RTS Threshold */
	httpap_tmp16 = GCONFIG_GetWifiRtsThreshold();
	httpap_tmp8 = HTTPAPI_Short2Str(httpap_tmp16, HTTP_TmpBuf);
	httpap_SetElementValue(RECORD_TEXT_rts_threshold, HTTP_TmpBuf, httpap_tmp8);
	
	/* Auto Power Control */
	httpap_tmp8 = GCONFIG_GetWifiAutoPowerCtrl();
	httpap_SetElementValue(RECORD_SELECT_auto_pwr_ctrl, &httpap_tmp8, 1);

	/* Protection Frame Type */
	httpap_tmp8 = GCONFIG_GetProtectionFrameType();
	httpap_SetElementValue(RECORD_SELECT_protfrm_type, &httpap_tmp8, 1);
	
	return HTTP_POST_OK;
} /* End of httpap_GetWiFiPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostWiFiPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostWiFiPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T i, j;
	
	if (httpap_CheckElementUpdate(RECORD_SELECT_network_mode) != HTTPAPI_ERR && 
		httpap_CheckElementUpdate(RECORD_TEXT_aes_passphrase) != HTTPAPI_ERR)
	{
		// Network mode //
		httpap_GetElementValue(RECORD_SELECT_network_mode, &pHttpap_buf);
		GCONFIG_SetWifiNetworkMode(*pHttpap_buf);
				
		// Channel //
		httpap_GetElementValue(RECORD_SELECT_channel, &pHttpap_buf);		
		GCONFIG_SetChannel(*pHttpap_buf);
	
		// Service Area Name/SSID //
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_ssid, &pHttpap_buf);		
		GCONFIG_SetSsid(pHttpap_buf, httpap_tmp8);
				
		// Secirity Mode //
		httpap_GetElementValue(RECORD_SELECT_security_mode, &pHttpap_buf);
		GCONFIG_SetWifiEncryptMode(*pHttpap_buf);
		
		// Key Length //	
		httpap_GetElementValue(RECORD_SELECT_key_length, &pHttpap_buf);
		GCONFIG_SetWifiWepKeyLength(*pHttpap_buf);
		i = *pHttpap_buf;
				
		// Key Index Select //
		httpap_GetElementValue(RECORD_SELECT_key_index, &pHttpap_buf);		
		GCONFIG_SetWifiWepKeyIndex(*pHttpap_buf);

		// WEP 64/128 bits key 1~4 //	
		for (j = 0; j < 4; j ++)
		{
			switch (j)
			{
			case 0:
				httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_key_index_0, &pHttpap_buf); break;
			case 1:
				httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_key_index_1, &pHttpap_buf); break;
			case 2:
				httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_key_index_2, &pHttpap_buf); break;
			case 3:
				httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_key_index_3, &pHttpap_buf); break;
			}

			if (httpap_tmp8)
			{
				if (httpap_tmp8 != 10 && httpap_tmp8 != 26)
					goto HTTPAP_POST_WIFI_ERR;
				if (!HTTPAPI_HexText2Num(HTTP_TmpBuf, pHttpap_buf, httpap_tmp8))
					goto HTTPAP_POST_WIFI_ERR;
					
				if (i)
					GCONFIG_SetWifiWep128Key(j, HTTP_TmpBuf);
				else
					GCONFIG_SetWifiWep64Key(j, HTTP_TmpBuf);
			}
		}
		// AES/TKIP Passphrase //	
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_aes_passphrase, &pHttpap_buf);
		if (httpap_tmp8 < 8 || httpap_tmp8 > 63)
			goto HTTPAP_POST_WIFI_ERR;			
		GCONFIG_SetWifiPreShareKey(pHttpap_buf, httpap_tmp8);
		
		goto HTTPAP_POST_WIFI_OK_SAVE_AND_REBOOT;
	}
	else if (httpap_CheckElementUpdate(RECORD_SELECT_tx_data_rate) != HTTPAPI_ERR && 
	         httpap_CheckElementUpdate(RECORD_SELECT_auto_pwr_ctrl) != HTTPAPI_ERR)			 
	{
		// TX Data Rate //	
		httpap_GetElementValue(RECORD_SELECT_tx_data_rate, &pHttpap_buf);		
		GCONFIG_SetWifiTxRate(*pHttpap_buf);
		
		// Transmission Power //	
		httpap_GetElementValue(RECORD_SELECT_tx_power, &pHttpap_buf);		
		GCONFIG_SetWifiTxPowerLevel(*pHttpap_buf);
		
		// Preamble //	
		httpap_GetElementValue(RECORD_SELECT_preamble, &pHttpap_buf);		
		GCONFIG_SetWifiPreambleMode(*pHttpap_buf);		
				
		// Beacon Interval //	
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_beacon_intv, &pHttpap_buf);		
		httpap_tmp16 = HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8);		
		GCONFIG_SetWifiBcnInterval(httpap_tmp16);		
		
		// RTS Threshold //						
		httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_rts_threshold, &pHttpap_buf);		
		httpap_tmp16 = HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8);		
		GCONFIG_SetWifiRtsThreshold(httpap_tmp16);	
				
		// Auto Power Control //						
		httpap_tmp8 = httpap_GetElementValue(RECORD_SELECT_auto_pwr_ctrl, &pHttpap_buf);		
		GCONFIG_SetWifiAutoPowerCtrl(*pHttpap_buf);		

		// Protection Frame Type //
		httpap_GetElementValue(RECORD_SELECT_protfrm_type, &pHttpap_buf);		
		GCONFIG_SetProtectionFrameType(*pHttpap_buf);		
	
		goto HTTPAP_POST_WIFI_OK_SAVE_AND_REBOOT;
	}	
	
HTTPAP_POST_WIFI_ERR:	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
											 ,"Settings validation error!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);	
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);
	
	return HTTP_POST_ERR;
	
HTTPAP_POST_WIFI_OK_SAVE_AND_REBOOT:
	// Store configuration to flash ROM //
	GCONFIG_WriteConfigData();
				
	httpdap.Cmd = HTTPDAP_REBOOT_CMD;
	httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 20 : 15;//select delay time for DHCP enable or disable. //			
	
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
											// warning message, delay time, delay time unit, delay time visible, redirect URL //
										 ,"Device reboot! please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);
	return HTTP_POST_ERR;
} /* End of httpap_PostWiFiPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_GetWiFiWizardPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_GetWiFiWizardPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T i, *pbuf1, *pbuf2;
	
	if (pConn->FileId == FILE_WIFIWZ_SYS)
	{	
		/* Network mode */
		httpap_tmp8 = GCONFIG_GetWifiNetworkMode() ? 1:0;
		httpap_SetElementValue(RECORD_SELECT_wznetworkmode, &httpap_tmp8, 1);
	
		/* Channel */
		httpap_tmp8 = GCONFIG_GetChannel();
		httpap_SetElementValue(RECORD_SELECT_wzchannel, &httpap_tmp8, 1);
	
		/* Service Area Name/SSID */
		httpap_tmp8 = GCONFIG_GetSsid(HTTP_TmpBuf);
		httpap_SetElementValue(RECORD_TEXT_wzssid, HTTP_TmpBuf, httpap_tmp8);
			
		/* Secirity Mode */
		httpap_tmp8 = GCONFIG_GetWifiEncryptMode();// % 5;
		httpap_SetElementValue(RECORD_SELECT_wzscrtmode, &httpap_tmp8, 1);
	}
	
	if (pConn->FileId == FILE_WIFIWZ_WEP)
	{	
		/* Key Length */	
		httpap_tmp8 = GCONFIG_GetWifiWepKeyLength() ? 1:0;
		httpap_SetElementValue(RECORD_SELECT_wzkeylength, &httpap_tmp8, 1);
	
		if (httpap_tmp8)
		{
			/* WEP 128bits Key 0~3 */
			GCONFIG_GetWifiWep128Key(0, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex0, HTTP_TmpBuf+100, 26);

			GCONFIG_GetWifiWep128Key(1, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex1, HTTP_TmpBuf+100, 26);

			GCONFIG_GetWifiWep128Key(2, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex2, HTTP_TmpBuf+100, 26);

			GCONFIG_GetWifiWep128Key(3, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 13);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex3, HTTP_TmpBuf+100, 26);
		}
		else
		{
			/* WEP 64bits Key 0~3 */
			GCONFIG_GetWifiWep64Key(0, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex0, HTTP_TmpBuf+100, 10);

			GCONFIG_GetWifiWep64Key(1, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex1, HTTP_TmpBuf+100, 10);

			GCONFIG_GetWifiWep64Key(2, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex2, HTTP_TmpBuf+100, 10);

			GCONFIG_GetWifiWep64Key(3, HTTP_TmpBuf);
			HTTPAPI_Num2HexText(HTTP_TmpBuf+100, HTTP_TmpBuf, 5);
			httpap_SetElementValue(RECORD_TEXT_wzkeyindex3, HTTP_TmpBuf+100, 10);
		}	
		
		/* Key Index Select */
		httpap_tmp8 = GCONFIG_GetWifiWepKeyIndex();// % 4;
		httpap_SetElementValue(RECORD_SELECT_wzkeyindex, &httpap_tmp8, 1);
		
		/* for WEP64bits and WEP128bit dynamic option */
		pbuf1 = HTTP_TmpBuf + 400;
		pbuf2 = HTTP_TmpBuf + 450;	
		pHttpap_buf = HTTP_TmpBuf;
		httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "<input type=\"hidden\" id=\"wep64_info\" value=\"");// Build hidden element start //		
		for (i = 0; i < 4; i ++)
		{	
			GCONFIG_GetWifiWep64Key(i, pbuf1);
			HTTPAPI_Num2HexText(pbuf2, pbuf1, 5);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", pbuf2);	
		}
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "\"/>");/* Build hidden element end */			
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "<input type=\"hidden\" id=\"wep128_info\" value=\"");// Build hidden element start //			
		for (i = 0; i < 4; i ++)
		{	
			GCONFIG_GetWifiWep128Key(i, pbuf1);
			HTTPAPI_Num2HexText(pbuf2, pbuf1, 13);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", pbuf2);
		}
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "\"/>");/* Build hidden element end */	
		HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, pConn->FileId, pConn);										
		return HTTP_POST_ERR;		
	}
	
	if (pConn->FileId == FILE_WIFIWZ_WPA)
	{	
	
		/* AES/TKIP Passphrase */	
		httpap_tmp8 = GCONFIG_GetWifiPreShareKey(HTTP_TmpBuf);	
		httpap_SetElementValue(RECORD_TEXT_wzaes, HTTP_TmpBuf, httpap_tmp8);
	}
	
	if (pConn->FileId == FILE_WIFIWZ_ADV)
	{	
		/* TX Data Rate */	
		httpap_tmp8 = GCONFIG_GetWifiTxRate();//%sizof(IO_WiFiSupportTxRate);
		httpap_SetElementValue(RECORD_SELECT_wztxdatarate, &httpap_tmp8, 1);
	
		/* Transmission Power */	
		httpap_tmp8 = GCONFIG_GetWifiTxPowerLevel();
		httpap_SetElementValue(RECORD_SELECT_wztxpower, &httpap_tmp8, 1);
	
		/* Preamble */	
		httpap_tmp8 = GCONFIG_GetWifiPreambleMode();
		httpap_SetElementValue(RECORD_SELECT_wzpreamble, &httpap_tmp8, 1);
	
		/* Beacon Interval */	
		httpap_tmp16 = GCONFIG_GetWifiBcnInterval();
		httpap_tmp8 = HTTPAPI_Short2Str(httpap_tmp16, HTTP_TmpBuf);
		httpap_SetElementValue(RECORD_TEXT_wzbeaconintv, HTTP_TmpBuf, httpap_tmp8);
	
		/* RTS Threshold */
		httpap_tmp16 = GCONFIG_GetWifiRtsThreshold();
		httpap_tmp8 = HTTPAPI_Short2Str(httpap_tmp16, HTTP_TmpBuf);
		httpap_SetElementValue(RECORD_TEXT_wzrtsthreshold, HTTP_TmpBuf, httpap_tmp8);
	
		/* Auto Power Control */
		httpap_tmp8 = GCONFIG_GetWifiAutoPowerCtrl();
		httpap_SetElementValue(RECORD_SELECT_wzautopwrctrl, &httpap_tmp8, 1);
		
		/* Protection Frame Type */
		httpap_tmp8 = GCONFIG_GetProtectionFrameType();
		httpap_SetElementValue(RECORD_SELECT_wzprotfrm_type, &httpap_tmp8, 1);		
	}
	
	if (pConn->FileId == FILE_WIFIWZ_STAT)
	{
		U8_T tmpbuf[40];
		U16_T tmp16;
	
		/* Build hidden element start */	
		pHttpap_buf = HTTP_TmpBuf;
		httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "<input type=\"hidden\" id=\"wifiwzstat\" value=\"");
		
		/* Network setting */
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", GCONFIG_GetWifiNetworkMode() ? "Ad-hoc" : "infrastructure");
		
		/* Channel */
		i = GCONFIG_GetChannel();
		if (i == 0)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "auto&");		
		else
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%bu&",IO_WiFiSupportedCh[i]);
		
		/* SSID */
		i = GCONFIG_GetSsid(tmpbuf);
		tmpbuf[i] = '\0';		
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", tmpbuf);
		
		/* Security Mode and key*/
		pbuf1 = HTTP_TmpBuf + 400;
		pbuf2 = HTTP_TmpBuf + 450;			
		i = GCONFIG_GetWifiEncryptMode();
		if (i == 0)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "no security&");
		else if (i == 1)
		{
			httpap_tmp8 = GCONFIG_GetWifiWepKeyIndex();
			GCONFIG_GetWifiWep64Key(httpap_tmp8, pbuf1);
			HTTPAPI_Num2HexText(pbuf2, pbuf1, 5);
			pbuf2[10] = '\0';
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "WEP64 bits, Index: %bu, key: %s&", httpap_tmp8, pbuf2);		
		}
		else if (i == 2)
		{
			httpap_tmp8 = GCONFIG_GetWifiWepKeyIndex();
			GCONFIG_GetWifiWep128Key(httpap_tmp8, pbuf1);
			HTTPAPI_Num2HexText(pbuf2, pbuf1, 13);
			pbuf2[26] = '\0';			
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "WEP128 bits, Index: %bu, key: %s&", httpap_tmp8, pbuf2);				
		}
		else if (i == 3)
		{
			httpap_tmp8 = GCONFIG_GetWifiPreShareKey(pbuf1);
			pbuf1[httpap_tmp8] = '\0';
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "TKIP, key: %s&", pbuf1);
		}
		else if (i == 4)
		{
			httpap_tmp8 = GCONFIG_GetWifiPreShareKey(pbuf1);
			pbuf1[httpap_tmp8] = '\0';			
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "AES, key: %s&", pbuf1);
		}
		else
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "unsupport&");		
			
		/* Tx data rate */
		i = GCONFIG_GetWifiTxRate();
		if (i == 0)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "auto&");
		else if (i == 1)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "1 Mbps&");		
		else if (i == 2)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "2 Mbps&");		
		else if (i == 3)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "5.5 Mbps&");		
		else if (i == 4)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "6 Mbps&");		
		else if (i == 5)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "9 Mbps&");		
		else if (i == 6)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "11 Mbps&");		
		else if (i == 7)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "12 Mbps&");		
		else if (i == 8)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "18 Mbps&");		
		else if (i == 9)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "24 Mbps&");		
		else if (i == 10)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "36 Mbps&");		
		else if (i == 11)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "48 Mbps&");	
		else if (i == 12)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "54 Mbps&");				
		else
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "unsupport&");		
			
		/* Tx power */
		i = GCONFIG_GetWifiTxPowerLevel();
		if (i == 0)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "100%%&");
		else if (i == 1)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "50%%&");				
		else if (i == 2)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "25%%&");				
		else if (i == 3)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "12.5%%&");										
		else if (i == 4)
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "6.25%%&");							
		else
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "unsupport&");		
		
		/* Preamble */	
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", GCONFIG_GetWifiPreambleMode() ? "auto":"long");
		
		/* Beacon Interval */
		tmp16 = GCONFIG_GetWifiBcnInterval();
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%u ms&", tmp16);

		/* RTS threshold */
		tmp16 = GCONFIG_GetWifiRtsThreshold();
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%u&", tmp16);
		
		/* Auto power control */
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", GCONFIG_GetWifiAutoPowerCtrl() ? "enable":"disable");

		/* Protection Frame Type */
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", GCONFIG_GetProtectionFrameType() ? "Self-CTS":"RTS");

		/* Build hidden element end and response message */	
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "\"/>");
		HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, pConn->FileId, pConn);	
		return HTTP_POST_ERR;	
	}
	return HTTP_POST_OK;
} /* End of httpap_GetWiFiWizardPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostWiFiWizardPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostWiFiWizardPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T i, j, *pTmp;
	
	if (pConn->FileId == FILE_WIFI_WZ)
	{
		if (httpap_CheckElementUpdateByName("Apply") != HTTPAPI_ERR)
		{
			// Save to flash memory //
			GCONFIG_WriteConfigData();		
			
			httpdap.Cmd = HTTPDAP_REBOOT_CMD;
			httpap_tmp8 = (GCONFIG_GetNetwork() & GCONFIG_NETWORK_DHCP_ENABLE) ? 20 : 15;//select delay time for DHCP enable or disable. //			
		
			pHttpap_buf = HTTP_TmpBuf;
			httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_SYSMSG);		
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
												// warning message, delay time, delay time unit, delay time visible, redirect URL //
												 ,"Device reboot! please Wait",httpap_tmp8,"sec","on",FSYS_Manage[pInfo->HomePage].FName);			
			HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_SYSMSG, pConn);		
			return HTTP_POST_ERR;			
		}
	}
		
	if (pConn->FileId == FILE_WIFIWZ_SYS)
	{
		if (httpap_CheckElementUpdate(RECORD_SELECT_wznetworkmode) == 0 && 
			httpap_CheckElementUpdate(RECORD_SELECT_wzscrtmode) == 3)	
		{
			// Network mode //
			httpap_GetElementValue(RECORD_SELECT_wznetworkmode, &pHttpap_buf);
			GCONFIG_SetWifiNetworkMode(*pHttpap_buf);
				
			// Channel //
			httpap_GetElementValue(RECORD_SELECT_wzchannel, &pHttpap_buf);		
			GCONFIG_SetChannel(*pHttpap_buf);
	
			// Service Area Name/SSID //
			httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzssid, &pHttpap_buf);		
			GCONFIG_SetSsid(pHttpap_buf, httpap_tmp8);
				
			// Secirity Mode //
			httpap_GetElementValue(RECORD_SELECT_wzscrtmode, &pHttpap_buf);
			GCONFIG_SetWifiEncryptMode(*pHttpap_buf);

			if (httpap_CheckElementUpdateByName("Finish") != HTTPAPI_ERR)
				HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_STAT, pConn);
			else
				HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ECRYTY, pConn);	
			return HTTP_POST_ERR;
		}
		goto HTTPAP_POST_WIFIWZ_ERR;
	}
	
	if (pConn->FileId == FILE_WIFIWZ_ECRYTY)
	{
		if (httpap_CheckElementUpdateByName("Back") != HTTPAPI_ERR)
		{
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_SYS, pConn);
			return HTTP_POST_ERR;		
		}		
		
		httpap_GetElementValue(RECORD_RADIO_wep, &pTmp);
		if (*pTmp == 0)
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_WEP, pConn);	
		else if (*pTmp == 1)
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_WPA, pConn);
		else
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ADV, pConn);
		return HTTP_POST_ERR;
	}	
	
	if (pConn->FileId == FILE_WIFIWZ_WEP)
	{
		if (httpap_CheckElementUpdateByName("Back") != HTTPAPI_ERR)
		{
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ECRYTY, pConn);
			return HTTP_POST_ERR;		
		}	
		
		if (httpap_CheckElementUpdate(RECORD_SELECT_wzkeylength) == 0 && 
	        httpap_CheckElementUpdate(RECORD_TEXT_wzkeyindex3) == 5)			 
		{
			// Key Length //	
			httpap_GetElementValue(RECORD_SELECT_wzkeylength, &pHttpap_buf);
			GCONFIG_SetWifiWepKeyLength(*pHttpap_buf);
			i = *pHttpap_buf;
				
			// Key Index Select //
			httpap_GetElementValue(RECORD_SELECT_wzkeyindex, &pHttpap_buf);		
			GCONFIG_SetWifiWepKeyIndex(*pHttpap_buf);

			// WEP 64/128 bits key 1~4 //	
			for (j = 0; j < 4; j ++)
			{
				switch (j)
				{
				case 0:
					httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzkeyindex0, &pHttpap_buf); break;
				case 1:
					httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzkeyindex1, &pHttpap_buf); break;
				case 2:
					httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzkeyindex2, &pHttpap_buf); break;
				case 3:
					httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzkeyindex3, &pHttpap_buf); break;
				}

				if (httpap_tmp8)
				{
					if (httpap_tmp8 != 10 && httpap_tmp8 != 26)
						goto HTTPAP_POST_WIFIWZ_ERR;
					if (!HTTPAPI_HexText2Num(HTTP_TmpBuf, pHttpap_buf, httpap_tmp8))
						goto HTTPAP_POST_WIFIWZ_ERR;
					if (i)
						GCONFIG_SetWifiWep128Key(j, HTTP_TmpBuf);
					else
						GCONFIG_SetWifiWep64Key(j, HTTP_TmpBuf);
				}
			}
			if (httpap_CheckElementUpdateByName("Finish") != HTTPAPI_ERR)
				HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_STAT, pConn);
			else
				HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ADV, pConn);	
			return HTTP_POST_ERR;
		}
		goto HTTPAP_POST_WIFIWZ_ERR;		
	}
	
	if (pConn->FileId == FILE_WIFIWZ_WPA)
	{
		if (httpap_CheckElementUpdateByName("Back") != HTTPAPI_ERR)
		{
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ECRYTY, pConn);
			return HTTP_POST_ERR;		
		}	

		if (httpap_CheckElementUpdate(RECORD_TEXT_wzaes) == 0)
		{
			// AES/TKIP Passphrase //	
			httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzaes, &pHttpap_buf);
			if (httpap_tmp8 < 8 || httpap_tmp8 > 63)
				goto HTTPAP_POST_WIFIWZ_ERR;			
			GCONFIG_SetWifiPreShareKey(pHttpap_buf, httpap_tmp8);
			
			if (httpap_CheckElementUpdateByName("Finish") != HTTPAPI_ERR)
				HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_STAT, pConn);
			else			
				HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ADV, pConn);	
			return HTTP_POST_ERR;
		}
		goto HTTPAP_POST_WIFIWZ_ERR;		
	}
	
	if (pConn->FileId == FILE_WIFIWZ_ADV)
	{
		if (httpap_CheckElementUpdateByName("Back") != HTTPAPI_ERR)
		{
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ECRYTY, pConn);
			return HTTP_POST_ERR;		
		}		
		if (httpap_CheckElementUpdate(RECORD_SELECT_wztxdatarate) == 0 && 
	        httpap_CheckElementUpdate(RECORD_SELECT_wzautopwrctrl) == 5)			 
		{
			// TX Data Rate //	
			httpap_GetElementValue(RECORD_SELECT_wztxdatarate, &pHttpap_buf);		
			GCONFIG_SetWifiTxRate(*pHttpap_buf);
		
			// Transmission Power //	
			httpap_GetElementValue(RECORD_SELECT_wztxpower, &pHttpap_buf);		
			GCONFIG_SetWifiTxPowerLevel(*pHttpap_buf);
		
			// Preamble //	
			httpap_GetElementValue(RECORD_SELECT_wzpreamble, &pHttpap_buf);		
			GCONFIG_SetWifiPreambleMode(*pHttpap_buf);		
				
			// Beacon Interval //	
			httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzbeaconintv, &pHttpap_buf);		
			httpap_tmp16 = HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8);		
			GCONFIG_SetWifiBcnInterval(httpap_tmp16);		
		
			// RTS Threshold //						
			httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_wzrtsthreshold, &pHttpap_buf);		
			httpap_tmp16 = HTTPAPI_Str2Short(pHttpap_buf, httpap_tmp8);		
			GCONFIG_SetWifiRtsThreshold(httpap_tmp16);	
				
			// Auto Power Control //						
			httpap_tmp8 = httpap_GetElementValue(RECORD_SELECT_wzautopwrctrl, &pHttpap_buf);		
			GCONFIG_SetWifiAutoPowerCtrl(*pHttpap_buf);	

			// Protection Frame Type //
			httpap_GetElementValue(RECORD_SELECT_wzprotfrm_type, &pHttpap_buf);		
			GCONFIG_SetProtectionFrameType(*pHttpap_buf);		
		
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_STAT, pConn);	
			return HTTP_POST_ERR;
		}
		goto HTTPAP_POST_WIFIWZ_ERR;		
	}
	
	if (pConn->FileId == FILE_WIFIWZ_STAT)
	{
		if (httpap_CheckElementUpdateByName("Back") != HTTPAPI_ERR)
		{
			HTTPAPI_ResponseRedirectMesssge(HTTP_TmpBuf, FILE_WIFIWZ_ADV, pConn);
			return HTTP_POST_ERR;		
		}	
		goto HTTPAP_POST_WIFIWZ_ERR;		
	}	
HTTPAP_POST_WIFIWZ_ERR:
	pHttpap_buf = HTTP_TmpBuf;
	httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_WIFIWZ_MSG);
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_Hidden, NULL, "sysmsg_setting" ,"%s&%bu&%s&%s&%s"
										// warning message, delay time, delay time unit, delay time visible, redirect URL //
										 ,"Settings validation error!",2,"sec","off",FSYS_Manage[pConn->FileId].FName);
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, FILE_WIFIWZ_MSG, pConn);
	return HTTP_POST_ERR;	
} /* End of httpap_PostWiFiWizardPage() */

/*
 * ----------------------------------------------------------------------------
 * Function Name: httpap_PostHctentPage()
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T httpap_PostHctentPage(HTTP_SERVER_CONN *pConn, HTTP_SERVER_INFO *pInfo)
{
	U8_T i, *pTmp, *pbuf1, *pbuf2;
	U8_T tmpbuf[20];
			
	if (httpap_CheckElementUpdate(RECORD_TEXT_cmd) == HTTPAPI_ERR)
		return HTTP_POST_OK;

	httpap_tmp8 = httpap_GetElementValue(RECORD_TEXT_cmd, &pHttpap_buf);
	
	if (!memcmp(pHttpap_buf,"getwep",httpap_tmp8))
	{
		/* for WEP64bits and WEP128bit dynamic option */
		pbuf1 = HTTP_TmpBuf + 400;
		pbuf2 = HTTP_TmpBuf + 450;	
		pHttpap_buf = HTTP_TmpBuf;
		httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_HCTENT);
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "<input type=\"hidden\" id=\"0\" value=\"");// Build hidden element start //		
		for (i = 0; i < 4; i ++)
		{	
			GCONFIG_GetWifiWep64Key(i, pbuf1);
			HTTPAPI_Num2HexText(pbuf2, pbuf1, 5);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", pbuf2);	
		}
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "\"/>");/* Build hidden element end */			
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "<input type=\"hidden\" id=\"1\" value=\"");// Build hidden element start //			
		for (i = 0; i < 4; i ++)
		{	
			GCONFIG_GetWifiWep128Key(i, pbuf1);
			HTTPAPI_Num2HexText(pbuf2, pbuf1, 13);
			httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", pbuf2);
		}
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "\"/>");/* Build hidden element end */	
		HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, pConn->FileId, pConn);										
		return HTTP_POST_ERR;
	}
	else if (!memcmp(pHttpap_buf,"getstatus",httpap_tmp8))
	{
	pHttpap_buf = (U8_T *)(GCONFIG_GetDeviceName());
	httpap_tmp8 = strlen(pHttpap_buf);
	if (httpap_tmp8 > 16)
		httpap_tmp8 = 16;
		memcpy(tmpbuf, pHttpap_buf, httpap_tmp8);
		tmpbuf[httpap_tmp8] = '\0';
	
	pHttpap_buf = HTTP_TmpBuf;
		httpap_tmp16 = HTTPAPI_BuildGenericHeaders(&pHttpap_buf, FILE_HCTENT);
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "<input type=\"hidden\" id=\"0\" value=\"");// Build hidden element start //	
		httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", tmpbuf);// Build device name //
	pTmp = (U8_T*)&(STOE_GetIPAddr());	
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%bu.%bu.%bu.%bu&"// Build local IP address //
											 ,pTmp[0],pTmp[1],pTmp[2],pTmp[3]);		
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", GCONFIG_VERSION_STRING);// Build mcpu firmware version //
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&", MWIFAPP_WiFiInfo.WcpuFwVersion);// Build wcpu firmware version //
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "0x%08lx%02bx%02bx&"// Build wcpu mac address //
											 ,*(U32_T*)(MWIFAPP_WiFiInfo.WcpuMAC), MWIFAPP_WiFiInfo.WcpuMAC[4], MWIFAPP_WiFiInfo.WcpuMAC[5]);	
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%bx&", HSUR2_GetModemStatus(1));// Build modem status //											 
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&"// Build protocol type //	
											 ,(GCONFIG_GetNetwork() & GCONFIG_NETWORK_PROTO_TCP)?"TCP":"UDP");									
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%s&"// Build connection type //			
											 ,(GS2W_GetTaskState() == GS2W_STATE_IDLE)?"Idle":"Connected");																	
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%lu&"// Build HUART TX byte counter //
											 ,(GS2W_CONN_UDP == GS2W_GetConnType())? GUDPDAT_GetTxBytes(0) : GTCPDAT_GetTxBytes(0));
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "%lu"// Build HUART RX byte counter //
											 ,(GS2W_CONN_UDP == GS2W_GetConnType())? GUDPDAT_GetRxBytes(0) : GTCPDAT_GetRxBytes(0));
	httpap_tmp16 += HTTPAPI_BuildMessageBody(&pHttpap_buf, HTTPAPI_FmtOnly, NULL, NULL, "\"/>");// Build hidden element end //	
	HTTPAPI_ResponseResource(HTTP_TmpBuf, httpap_tmp16, pConn->FileId, pConn);	
	return HTTP_POST_ERR;
	}
	
	return HTTP_POST_OK;	
} /* End of httpap_PostHctentPage() */

/* Add to support customers' application, end */

/* End of httpdap.c */
