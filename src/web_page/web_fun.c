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
 * Module Name: web_fun.c
 * Purpose:
 * Author:
 * Date:
 * Notes:
 * $Log: web_fun.c,v $
 *=============================================================================
 */

/* INCLUDE FILE DECLARATIONS */
#include "filesys.h"
#include "httpd.h"


/* GLOBAL VARIABLE DECLARATIONS */
const void *fun_tbl[] =
{
	FUN_TEXT_tftps_ip_b,
	FUN_TEXT_file_name_b,
	NULL,
	NULL,
	FUN_TEXT_tftps_ip,
	FUN_TEXT_file_name,
	NULL,
	FUN_TEXT_tftps_ip_w,
	FUN_TEXT_file_name_w,
	NULL,
	FUN_RADIO_dhcps_onoff,
	FUN_TEXT_dhcps_startip,
	FUN_TEXT_dhcps_endip,
	FUN_TEXT_dhcps_gateway,
	FUN_TEXT_dhcps_mask,
	FUN_TEXT_dsm_name,
	FUN_SELECT_dbr,
	FUN_SELECT_data,
	FUN_SELECT_parity,
	FUN_SELECT_stop,
	FUN_SELECT_flow,
	FUN_SELECT_rs485,
	FUN_SELECT_dhcp,
	FUN_TEXT_static_ip,
	FUN_TEXT_mask,
	FUN_TEXT_gateway_ip,
	FUN_TEXT_dns_ip,
	FUN_SELECT_conntype,
	FUN_TEXT_txtimer,
	FUN_SELECT_cs_mode,
	FUN_TEXT_s_lstport,
	FUN_TEXT_c_deshn,
	FUN_TEXT_c_desport,
	NULL,
	NULL,
	FUN_TEXT_cmd,
	FUN_TEXT_username,
	FUN_PASSWORD_password,
	NULL,
	FUN_TEXT_new_usn,
	FUN_PASSWORD_old_psw,
	FUN_PASSWORD_new_psw,
	FUN_PASSWORD_cfm_psw,
	FUN_SELECT_network_mode,
	FUN_SELECT_channel,
	FUN_TEXT_ssid,
	FUN_SELECT_security_mode,
	FUN_SELECT_key_length,
	FUN_SELECT_key_index,
	FUN_TEXT_key_index_0,
	FUN_TEXT_key_index_1,
	FUN_TEXT_key_index_2,
	FUN_TEXT_key_index_3,
	FUN_TEXT_aes_passphrase,
	FUN_SELECT_tx_data_rate,
	FUN_SELECT_tx_power,
	FUN_SELECT_preamble,
	FUN_TEXT_beacon_intv,
	FUN_TEXT_rts_threshold,
	FUN_SELECT_auto_pwr_ctrl,
	FUN_SELECT_protfrm_type,
	FUN_SELECT_wztxdatarate,
	FUN_SELECT_wztxpower,
	FUN_SELECT_wzpreamble,
	FUN_TEXT_wzbeaconintv,
	FUN_TEXT_wzrtsthreshold,
	FUN_SELECT_wzautopwrctrl,
	FUN_SELECT_wzprotfrm_type,
	NULL,
	NULL,
	FUN_RADIO_wep,
	FUN_SELECT_wznetworkmode,
	FUN_SELECT_wzchannel,
	FUN_TEXT_wzssid,
	FUN_SELECT_wzscrtmode,
	NULL,
	FUN_SELECT_wzkeylength,
	FUN_SELECT_wzkeyindex,
	FUN_TEXT_wzkeyindex0,
	FUN_TEXT_wzkeyindex1,
	FUN_TEXT_wzkeyindex2,
	FUN_TEXT_wzkeyindex3,
	FUN_TEXT_wzaes
};


/* STATIC VARIABLE DECLARATIONS */


/* LOCAL SUBPROGRAM DECLARATIONS */
/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_tftps_ip_b
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_tftps_ip_b (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_file_name_b
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_file_name_b (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_tftps_ip
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_tftps_ip (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_file_name
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_file_name (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_tftps_ip_w
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_tftps_ip_w (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_file_name_w
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_file_name_w (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_RADIO_dhcps_onoff
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_RADIO_dhcps_onoff (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_dhcps_startip
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_dhcps_startip (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_dhcps_endip
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_dhcps_endip (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_dhcps_gateway
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_dhcps_gateway (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_dhcps_mask
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_dhcps_mask (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_dsm_name
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_dsm_name (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_dbr
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_dbr (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_data
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_data (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_parity
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_parity (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_stop
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_stop (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_flow
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_flow (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_rs485
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_rs485 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_dhcp
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_dhcp (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_static_ip
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_static_ip (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_mask
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_mask (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_gateway_ip
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_gateway_ip (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_dns_ip
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_dns_ip (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_conntype
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_conntype (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_txtimer
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_txtimer (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_cs_mode
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_cs_mode (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_s_lstport
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_s_lstport (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_c_deshn
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_c_deshn (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_c_desport
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_c_desport (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_cmd
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_cmd (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_username
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_username (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_PASSWORD_password
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_PASSWORD_password (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_new_usn
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_new_usn (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_PASSWORD_old_psw
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_PASSWORD_old_psw (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_PASSWORD_new_psw
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_PASSWORD_new_psw (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_PASSWORD_cfm_psw
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_PASSWORD_cfm_psw (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_network_mode
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_network_mode (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_channel
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_channel (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_ssid
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_ssid (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_security_mode
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_security_mode (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_key_length
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_key_length (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_key_index
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_key_index (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_key_index_0
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_key_index_0 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_key_index_1
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_key_index_1 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_key_index_2
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_key_index_2 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_key_index_3
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_key_index_3 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_aes_passphrase
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_aes_passphrase (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_tx_data_rate
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_tx_data_rate (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_tx_power
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_tx_power (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_preamble
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_preamble (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_beacon_intv
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_beacon_intv (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_rts_threshold
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_rts_threshold (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_auto_pwr_ctrl
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_auto_pwr_ctrl (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_protfrm_type
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_protfrm_type (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wztxdatarate
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wztxdatarate (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wztxpower
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wztxpower (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzpreamble
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzpreamble (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzbeaconintv
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzbeaconintv (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzrtsthreshold
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzrtsthreshold (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzautopwrctrl
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzautopwrctrl (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzprotfrm_type
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzprotfrm_type (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_RADIO_wep
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_RADIO_wep (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wznetworkmode
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wznetworkmode (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzchannel
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzchannel (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzssid
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzssid (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzscrtmode
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzscrtmode (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzkeylength
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzkeylength (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_SELECT_wzkeyindex
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_SELECT_wzkeyindex (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzkeyindex0
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzkeyindex0 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzkeyindex1
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzkeyindex1 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzkeyindex2
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzkeyindex2 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzkeyindex3
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzkeyindex3 (void *pWebData)
{
	// TODO: Add your specialized code here

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FUN_TEXT_wzaes
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FUN_TEXT_wzaes (void *pWebData)
{
	// TODO: Add your specialized code here

}

/* End of web_fun.c */