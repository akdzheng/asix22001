/*
 ******************************************************************************
 *     Copyright (c) 2010 ASIX Electronic Corporation      All rights reserved.
 *
 *     This is unpublished proprietary source code of ASIX Electronic Corporation
 *
 *     The copyright notice above does not evidence any actual or intended
 *     publication of such source code.
 ******************************************************************************
 */
/*=============================================================================
 * Module Name: filesys.h
 * Purpose:
 * Author:
 * Date:
 * Notes:
 * $Log: filesys.h,v $
 *=============================================================================
 */

#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

/* INCLUDE FILE DECLARATIONS */
#include "types.h"

/* NAMING CONSTANT DECLARATIONS */
#define FILE_ADVANCED			2
#define FILE_AXCSS0			3
#define FILE_AXCSS1			4
#define FILE_AXJS0			5
#define FILE_BASIC			6
#define FILE_HCTENT			7
#define FILE_INDEX			8
#define FILE_SECURITY			9
#define FILE_STATUS			10
#define FILE_SYSCONFIRM			11
#define FILE_SYSMSG			12
#define FILE_WIFI			13
#define FILE_WIFIWZ_ADV			14
#define FILE_WIFIWZ_ECRYTY			15
#define FILE_WIFIWZ_MSG			16
#define FILE_WIFIWZ_STAT			17
#define FILE_WIFIWZ_SYS			18
#define FILE_WIFIWZ_WEP			19
#define FILE_WIFIWZ_WPA			20
#define FILE_WIFI_WZ			21

#define MAX_STORE_FILE_NUM	22
#define MAX_FILE_NAME_LEN	30

#define FILE_TYPE_HTML		0
#define FILE_TYPE_BIN		1
#define FILE_TYPE_JS		2
#define FILE_TYPE_CSS		3
#define FILE_TYPE_CGI		4

#define MAX_POST_RECORDS	83
#define MAX_POST_BUF_SUBMIT	10
#define MAX_POST_BUF_TEXT	38
#define MAX_POST_BUF_PASSWORD	4
#define MAX_POST_BUF_TAG	0
#define MAX_POST_BUF_RADIO	2
#define MAX_POST_BUF_SELECT	29
#define MAX_POST_NAME_LEN	15
#define MAX_POST_VALUE_LEN	64
#define NORM_POST_VALUE_LEN	16

#define MAX_SELECT_OPTION_NUM	13
#define MAX_RADIO_OPTION_NUM	3

#define POST_TYPE_SUBMIT	1
#define POST_TYPE_TEXT		2
#define POST_TYPE_RADIO		3
#define POST_TYPE_SELECT	4
#define POST_TYPE_TAG   	5
#define POST_TYPE_PASSWORD	6

#define RECORD_TEXT_tftps_ip_b		0
#define RECORD_TEXT_file_name_b		1
#define RECORD_TEXT_tftps_ip		4
#define RECORD_TEXT_file_name		5
#define RECORD_TEXT_tftps_ip_w		7
#define RECORD_TEXT_file_name_w		8
#define RECORD_RADIO_dhcps_onoff		10
#define RECORD_TEXT_dhcps_startip		11
#define RECORD_TEXT_dhcps_endip		12
#define RECORD_TEXT_dhcps_gateway		13
#define RECORD_TEXT_dhcps_mask		14
#define RECORD_TEXT_dsm_name		15
#define RECORD_SELECT_dbr		16
#define RECORD_SELECT_data		17
#define RECORD_SELECT_parity		18
#define RECORD_SELECT_stop		19
#define RECORD_SELECT_flow		20
#define RECORD_SELECT_rs485		21
#define RECORD_SELECT_dhcp		22
#define RECORD_TEXT_static_ip		23
#define RECORD_TEXT_mask		24
#define RECORD_TEXT_gateway_ip		25
#define RECORD_TEXT_dns_ip		26
#define RECORD_SELECT_conntype		27
#define RECORD_TEXT_txtimer		28
#define RECORD_SELECT_cs_mode		29
#define RECORD_TEXT_s_lstport		30
#define RECORD_TEXT_c_deshn		31
#define RECORD_TEXT_c_desport		32
#define RECORD_TEXT_cmd		35
#define RECORD_TEXT_username		36
#define RECORD_PASSWORD_password		37
#define RECORD_TEXT_new_usn		39
#define RECORD_PASSWORD_old_psw		40
#define RECORD_PASSWORD_new_psw		41
#define RECORD_PASSWORD_cfm_psw		42
#define RECORD_SELECT_network_mode		43
#define RECORD_SELECT_channel		44
#define RECORD_TEXT_ssid		45
#define RECORD_SELECT_security_mode		46
#define RECORD_SELECT_key_length		47
#define RECORD_SELECT_key_index		48
#define RECORD_TEXT_key_index_0		49
#define RECORD_TEXT_key_index_1		50
#define RECORD_TEXT_key_index_2		51
#define RECORD_TEXT_key_index_3		52
#define RECORD_TEXT_aes_passphrase		53
#define RECORD_SELECT_tx_data_rate		54
#define RECORD_SELECT_tx_power		55
#define RECORD_SELECT_preamble		56
#define RECORD_TEXT_beacon_intv		57
#define RECORD_TEXT_rts_threshold		58
#define RECORD_SELECT_auto_pwr_ctrl		59
#define RECORD_SELECT_protfrm_type		60
#define RECORD_SELECT_wztxdatarate		61
#define RECORD_SELECT_wztxpower		62
#define RECORD_SELECT_wzpreamble		63
#define RECORD_TEXT_wzbeaconintv		64
#define RECORD_TEXT_wzrtsthreshold		65
#define RECORD_SELECT_wzautopwrctrl		66
#define RECORD_SELECT_wzprotfrm_type		67
#define RECORD_RADIO_wep		70
#define RECORD_SELECT_wznetworkmode		71
#define RECORD_SELECT_wzchannel		72
#define RECORD_TEXT_wzssid		73
#define RECORD_SELECT_wzscrtmode		74
#define RECORD_SELECT_wzkeylength		76
#define RECORD_SELECT_wzkeyindex		77
#define RECORD_TEXT_wzkeyindex0		78
#define RECORD_TEXT_wzkeyindex1		79
#define RECORD_TEXT_wzkeyindex2		80
#define RECORD_TEXT_wzkeyindex3		81
#define RECORD_TEXT_wzaes		82

/* TYPE DECLARATIONS */
/*-------------------------------------------------------------*/
typedef struct _FILE_MANAGEMEMT
{
	U8_T			Occupy;
	U8_T			FType;
	U8_T			FName[MAX_FILE_NAME_LEN];
	U16_T			FileSize;
	U8_T			*PBuf;
	U8_T			CgiRef;
	void			*CgiCall;

} FILE_MANAGEMEMT;

/*-------------------------------------------------------------*/
typedef struct _POST_RECORD
{
	U8_T			Occupy;
	U8_T			PostType;
	U8_T			Name[MAX_POST_NAME_LEN];
	U8_T			NameLen;
	U8_T			FileIndex;
	U8_T			UpdateSelf;
	void XDATA*		PValue;

} POST_RECORD;

/*-------------------------------------------------------------*/
typedef struct _BUF_SUBMIT
{
	U8_T			Value[NORM_POST_VALUE_LEN];
	U8_T			DefaultVlaueLen;
	U8_T			IsApply;

} BUF_SUBMIT;

/*-------------------------------------------------------------*/
#if (MAX_POST_BUF_RADIO)
typedef struct _BUF_RADIO
{
	U16_T			Offset[MAX_RADIO_OPTION_NUM];
	U8_T			Value[MAX_RADIO_OPTION_NUM][NORM_POST_VALUE_LEN];
	U8_T			Length[MAX_RADIO_OPTION_NUM];
	U8_T			Count;
	U8_T			DefaultSet;
	U8_T			CurrentSet;
	U8_T			UserSet;

} BUF_RADIO;
#endif

/*-------------------------------------------------------------*/
typedef struct _BUF_TEXT
{
	U16_T			Offset;
	U8_T			*CurrValue;
	U8_T			*UserValue;
	U8_T			DefaultLength;
	U8_T			CurrLength;
	U8_T			UserLength;

} BUF_TEXT;

/*-------------------------------------------------------------*/
typedef struct _BUF_PASSWORD
{
	U16_T			Offset;
	U8_T			*CurrValue;
	U8_T			*UserValue;
	U8_T			DefaultLength;
	U8_T			CurrLength;
	U8_T			UserLength;

} BUF_PASSWORD;

/*-------------------------------------------------------------*/
typedef struct _BUF_TAG
{
	U16_T			Offset;
	U8_T			CurrValue[NORM_POST_VALUE_LEN];
	U8_T			DefaultLength;
	U8_T			CurrLength;

} BUF_TAG;

/*-------------------------------------------------------------*/
#if (MAX_POST_BUF_SELECT)
typedef struct _BUF_SELECT
{
	U16_T			Offset[MAX_SELECT_OPTION_NUM];
	U8_T			Count;
	U8_T			DefaultSet;
	U8_T			CurrentSet;
	U8_T			UserSet;

} BUF_SELECT;
#endif
/*-------------------------------------------------------------*/
 typedef struct {
	U8_T* name;
	U8_T* val;
} entry;
	extern entry entries[20];
	extern U8_T num_parms;

/*-------------------------------------------------------------*/

/* EXPORTED SUBPROGRAM SPECIFICATIONS */
#if (MAX_STORE_FILE_NUM)
extern FILE_MANAGEMEMT const FAR FSYS_Manage[MAX_STORE_FILE_NUM];
#endif
#if (MAX_POST_RECORDS)
extern POST_RECORD XDATA POST_Record[MAX_POST_RECORDS];
#endif

void FSYS_Init(void);
U8_T FSYS_FindFile(U8_T XDATA*);
U16_T FSYS_HtmlExpansion(U8_T);

const void *fun_tbl[];

void FUN_TEXT_tftps_ip_b (void *pWebData);
void FUN_TEXT_file_name_b (void *pWebData);
void FUN_TEXT_tftps_ip (void *pWebData);
void FUN_TEXT_file_name (void *pWebData);
void FUN_TEXT_tftps_ip_w (void *pWebData);
void FUN_TEXT_file_name_w (void *pWebData);
void FUN_RADIO_dhcps_onoff (void *pWebData);
void FUN_TEXT_dhcps_startip (void *pWebData);
void FUN_TEXT_dhcps_endip (void *pWebData);
void FUN_TEXT_dhcps_gateway (void *pWebData);
void FUN_TEXT_dhcps_mask (void *pWebData);
void FUN_TEXT_dsm_name (void *pWebData);
void FUN_SELECT_dbr (void *pWebData);
void FUN_SELECT_data (void *pWebData);
void FUN_SELECT_parity (void *pWebData);
void FUN_SELECT_stop (void *pWebData);
void FUN_SELECT_flow (void *pWebData);
void FUN_SELECT_rs485 (void *pWebData);
void FUN_SELECT_dhcp (void *pWebData);
void FUN_TEXT_static_ip (void *pWebData);
void FUN_TEXT_mask (void *pWebData);
void FUN_TEXT_gateway_ip (void *pWebData);
void FUN_TEXT_dns_ip (void *pWebData);
void FUN_SELECT_conntype (void *pWebData);
void FUN_TEXT_txtimer (void *pWebData);
void FUN_SELECT_cs_mode (void *pWebData);
void FUN_TEXT_s_lstport (void *pWebData);
void FUN_TEXT_c_deshn (void *pWebData);
void FUN_TEXT_c_desport (void *pWebData);
void FUN_TEXT_cmd (void *pWebData);
void FUN_TEXT_username (void *pWebData);
void FUN_PASSWORD_password (void *pWebData);
void FUN_TEXT_new_usn (void *pWebData);
void FUN_PASSWORD_old_psw (void *pWebData);
void FUN_PASSWORD_new_psw (void *pWebData);
void FUN_PASSWORD_cfm_psw (void *pWebData);
void FUN_SELECT_network_mode (void *pWebData);
void FUN_SELECT_channel (void *pWebData);
void FUN_TEXT_ssid (void *pWebData);
void FUN_SELECT_security_mode (void *pWebData);
void FUN_SELECT_key_length (void *pWebData);
void FUN_SELECT_key_index (void *pWebData);
void FUN_TEXT_key_index_0 (void *pWebData);
void FUN_TEXT_key_index_1 (void *pWebData);
void FUN_TEXT_key_index_2 (void *pWebData);
void FUN_TEXT_key_index_3 (void *pWebData);
void FUN_TEXT_aes_passphrase (void *pWebData);
void FUN_SELECT_tx_data_rate (void *pWebData);
void FUN_SELECT_tx_power (void *pWebData);
void FUN_SELECT_preamble (void *pWebData);
void FUN_TEXT_beacon_intv (void *pWebData);
void FUN_TEXT_rts_threshold (void *pWebData);
void FUN_SELECT_auto_pwr_ctrl (void *pWebData);
void FUN_SELECT_protfrm_type (void *pWebData);
void FUN_SELECT_wztxdatarate (void *pWebData);
void FUN_SELECT_wztxpower (void *pWebData);
void FUN_SELECT_wzpreamble (void *pWebData);
void FUN_TEXT_wzbeaconintv (void *pWebData);
void FUN_TEXT_wzrtsthreshold (void *pWebData);
void FUN_SELECT_wzautopwrctrl (void *pWebData);
void FUN_SELECT_wzprotfrm_type (void *pWebData);
void FUN_RADIO_wep (void *pWebData);
void FUN_SELECT_wznetworkmode (void *pWebData);
void FUN_SELECT_wzchannel (void *pWebData);
void FUN_TEXT_wzssid (void *pWebData);
void FUN_SELECT_wzscrtmode (void *pWebData);
void FUN_SELECT_wzkeylength (void *pWebData);
void FUN_SELECT_wzkeyindex (void *pWebData);
void FUN_TEXT_wzkeyindex0 (void *pWebData);
void FUN_TEXT_wzkeyindex1 (void *pWebData);
void FUN_TEXT_wzkeyindex2 (void *pWebData);
void FUN_TEXT_wzkeyindex3 (void *pWebData);
void FUN_TEXT_wzaes (void *pWebData);

#endif /* End of __FILE_SYSTEM_H__ */


/* End of filesys.h */