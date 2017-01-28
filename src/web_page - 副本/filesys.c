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
 * Module Name: filesys.c
 * Purpose:
 * Author:
 * Date:
 * Notes:
 * $Log: filesys.c,v $
 *=============================================================================
 */

/* INCLUDE FILE DECLARATIONS */
#include "filesys.h"
#include "httpd.h"


/* GLOBAL VARIABLE DECLARATIONS */
#if (MAX_POST_RECORDS)
POST_RECORD XDATA POST_Record[MAX_POST_RECORDS];
#endif
#if (MAX_POST_BUF_SUBMIT)
static BUF_SUBMIT XDATA BUF_Submit[MAX_POST_BUF_SUBMIT];
#endif
#if (MAX_POST_BUF_RADIO)
static BUF_RADIO XDATA BUF_Radio[MAX_POST_BUF_RADIO];
#endif
#if (MAX_POST_BUF_TEXT)
static BUF_TEXT XDATA BUF_Text[MAX_POST_BUF_TEXT];
#endif
#if (MAX_POST_BUF_PASSWORD)
static BUF_PASSWORD XDATA BUF_Password[MAX_POST_BUF_PASSWORD];
#endif
#if (MAX_POST_BUF_TAG)
static BUF_TAG XDATA BUF_Tag[MAX_POST_BUF_TAG];
#endif
#if (MAX_POST_BUF_SELECT)
static BUF_SELECT XDATA BUF_Select[MAX_POST_BUF_SELECT];
#endif


/* STATIC VARIABLE DECLARATIONS */
extern U8_T const FAR WebImg_asix[];
extern U8_T const FAR WebImg_Logo[];
extern U8_T const FAR WebImg_tbbg[];
extern U8_T const FAR WebAdvanced[];
extern U8_T const FAR WebAxcss0[];
extern U8_T const FAR WebAxcss1[];
extern U8_T const FAR WebAxjs0[];
extern U8_T const FAR WebBasic[];
extern U8_T const FAR WebHctent[];
extern U8_T const FAR WebIndex[];
extern U8_T const FAR WebSecurity[];
extern U8_T const FAR WebStatus[];
extern U8_T const FAR WebSysconfirm[];
extern U8_T const FAR WebSysmsg[];
extern U8_T const FAR WebWifi[];
extern U8_T const FAR WebWifiwz_adv[];
extern U8_T const FAR WebWifiwz_ecryty[];
extern U8_T const FAR WebWifiwz_msg[];
extern U8_T const FAR WebWifiwz_stat[];
extern U8_T const FAR WebWifiwz_sys[];
extern U8_T const FAR WebWifiwz_wep[];
extern U8_T const FAR WebWifiwz_wpa[];
extern U8_T const FAR WebWifi_wz[];

#if (MAX_STORE_FILE_NUM)
FILE_MANAGEMEMT const FAR FSYS_Manage[MAX_STORE_FILE_NUM] = 
{
    /* for file 0 */ {1, 1, "img_asix.gif", 1993, WebImg_asix, 0, (void*)0},
    /* for file 1 */ {1, 1, "img_Logo.jpg", 2841, WebImg_Logo, 1, (void*)0},
    /* for file 2 */ {1, 1, "img_tbbg.gif", 200, WebImg_tbbg, 2, (void*)0},
    /* for file 3 */ {1, 0, "advanced.htm", 6394, WebAdvanced, 3, (void*)0},
    /* for file 4 */ {1, 3, "axcss0.css", 1790, WebAxcss0, 4, (void*)0},
    /* for file 5 */ {1, 3, "axcss1.css", 1344, WebAxcss1, 5, (void*)0},
    /* for file 6 */ {1, 2, "axjs0.js", 5938, WebAxjs0, 6, (void*)0},
    /* for file 7 */ {1, 0, "basic.htm", 5650, WebBasic, 7, (void*)0},
    /* for file 8 */ {1, 0, "hctent.htm", 609, WebHctent, 8, (void*)0},
    /* for file 9 */ {1, 0, "index.htm", 1390, WebIndex, 9, (void*)0},
    /* for file 10 */ {1, 0, "security.htm", 3558, WebSecurity, 10, (void*)0},
    /* for file 11 */ {1, 0, "status.htm", 4291, WebStatus, 11, (void*)0},
    /* for file 12 */ {1, 0, "sysconfirm.htm", 2160, WebSysconfirm, 12, (void*)0},
    /* for file 13 */ {1, 0, "sysmsg.htm", 2232, WebSysmsg, 13, (void*)0},
    /* for file 14 */ {1, 0, "wifi.htm", 7637, WebWifi, 14, (void*)0},
    /* for file 15 */ {1, 0, "wifiwz_adv.htm", 5165, WebWifiwz_adv, 15, (void*)0},
    /* for file 16 */ {1, 0, "wifiwz_ecryty.htm", 1997, WebWifiwz_ecryty, 16, (void*)0},
    /* for file 17 */ {1, 0, "wifiwz_msg.htm", 1819, WebWifiwz_msg, 17, (void*)0},
    /* for file 18 */ {1, 0, "wifiwz_stat.htm", 3001, WebWifiwz_stat, 18, (void*)0},
    /* for file 19 */ {1, 0, "wifiwz_sys.htm", 4493, WebWifiwz_sys, 19, (void*)0},
    /* for file 20 */ {1, 0, "wifiwz_wep.htm", 4987, WebWifiwz_wep, 20, (void*)0},
    /* for file 21 */ {1, 0, "wifiwz_wpa.htm", 2654, WebWifiwz_wpa, 21, (void*)0},
    /* for file 22 */ {1, 0, "wifi_wz.htm", 1737, WebWifi_wz, 22, (void*)0},
};
#endif

/* LOCAL SUBPROGRAM DECLARATIONS */
static void fsys_InitPostRecord(void);

/*
 * ----------------------------------------------------------------------------
 * Function Name: FSYS_Init
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void FSYS_Init(void)
{


	fsys_InitPostRecord();
}

/*
 * ----------------------------------------------------------------------------
 * Function Name: fsys_InitPostRecord
 * Purpose:
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
void fsys_InitPostRecord(void)
{
	BUF_SUBMIT XDATA*	pSubmit;
	BUF_TEXT XDATA*		pText;
	BUF_PASSWORD XDATA*		pPassword;
	BUF_TAG XDATA*		pTag;
#if (MAX_POST_BUF_RADIO)
	BUF_RADIO XDATA*	pRadio;
#endif
#if (MAX_POST_BUF_SELECT)
	BUF_SELECT XDATA*	pSelect;
#endif
	U8_T				index;

	static U8_T XDATA postbuf_text0[2][16];
	static U8_T XDATA postbuf_text1[2][64];
	static U8_T XDATA postbuf_text2[2][16];
	static U8_T XDATA postbuf_text3[2][64];
	static U8_T XDATA postbuf_text4[2][16];
	static U8_T XDATA postbuf_text5[2][64];
	static U8_T XDATA postbuf_text6[2][36];
	static U8_T XDATA postbuf_text7[2][36];
	static U8_T XDATA postbuf_text8[2][36];
	static U8_T XDATA postbuf_text9[2][36];
	static U8_T XDATA postbuf_text10[2][36];
	static U8_T XDATA postbuf_text11[2][16];
	static U8_T XDATA postbuf_text12[2][16];
	static U8_T XDATA postbuf_text13[2][16];
	static U8_T XDATA postbuf_text14[2][16];
	static U8_T XDATA postbuf_text15[2][17];
	static U8_T XDATA postbuf_text16[2][16];
	static U8_T XDATA postbuf_text17[2][16];
	static U8_T XDATA postbuf_text18[2][16];
	static U8_T XDATA postbuf_text19[2][16];
	static U8_T XDATA postbuf_text20[2][6];
	static U8_T XDATA postbuf_text21[2][6];
	static U8_T XDATA postbuf_text22[2][36];
	static U8_T XDATA postbuf_text23[2][6];
	static U8_T XDATA postbuf_text24[2][16];
	static U8_T XDATA postbuf_text25[2][17];
	static U8_T XDATA postbuf_text26[2][16];
	static U8_T XDATA postbuf_text27[2][16];
	static U8_T XDATA postbuf_text28[2][16];
	static U8_T XDATA postbuf_text29[2][16];
	static U8_T XDATA postbuf_text30[2][16];
	static U8_T XDATA postbuf_text31[2][32];
	static U8_T XDATA postbuf_text32[2][27];
	static U8_T XDATA postbuf_text33[2][27];
	static U8_T XDATA postbuf_text34[2][27];
	static U8_T XDATA postbuf_text35[2][27];
	static U8_T XDATA postbuf_text36[2][64];
	static U8_T XDATA postbuf_text37[2][7];
	static U8_T XDATA postbuf_text38[2][7];
	static U8_T XDATA postbuf_text39[2][7];
	static U8_T XDATA postbuf_text40[2][7];
	static U8_T XDATA postbuf_text41[2][32];
	static U8_T XDATA postbuf_text42[2][27];
	static U8_T XDATA postbuf_text43[2][27];
	static U8_T XDATA postbuf_text44[2][27];
	static U8_T XDATA postbuf_text45[2][27];
	static U8_T XDATA postbuf_text46[2][64];
	static U8_T XDATA postbuf_password0[2][17];
	static U8_T XDATA postbuf_password1[2][16];
	static U8_T XDATA postbuf_password2[2][16];
	static U8_T XDATA postbuf_password3[2][16];

	for (index = 1; index < MAX_POST_RECORDS; index++)
		POST_Record[index].Occupy = 0;

	/* for record 0 */
	POST_Record[0].Occupy = 1;
	POST_Record[0].PostType = POST_TYPE_TEXT;
	POST_Record[0].Name[0] = 't';
	POST_Record[0].Name[1] = 'f';
	POST_Record[0].Name[2] = 't';
	POST_Record[0].Name[3] = 'p';
	POST_Record[0].Name[4] = 's';
	POST_Record[0].Name[5] = '_';
	POST_Record[0].Name[6] = 'i';
	POST_Record[0].Name[7] = 'p';
	POST_Record[0].Name[8] = '_';
	POST_Record[0].Name[9] = 'b';
	POST_Record[0].NameLen = 10;
	POST_Record[0].FileIndex = 3;
	POST_Record[0].UpdateSelf = FALSE;
	POST_Record[0].PValue = &BUF_Text[0];
	pText = POST_Record[0].PValue;
	pText->CurrValue = postbuf_text0[0];
	pText->UserValue = postbuf_text0[1];
	pText->Offset = 1336;
	pText->DefaultLength = 13;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '8';
	pText->CurrValue[11] = '0';
	pText->CurrValue[12] = '"';
	pText->CurrLength = 13;

	/* for record 1 */
	POST_Record[1].Occupy = 1;
	POST_Record[1].PostType = POST_TYPE_TEXT;
	POST_Record[1].Name[0] = 'f';
	POST_Record[1].Name[1] = 'i';
	POST_Record[1].Name[2] = 'l';
	POST_Record[1].Name[3] = 'e';
	POST_Record[1].Name[4] = '_';
	POST_Record[1].Name[5] = 'n';
	POST_Record[1].Name[6] = 'a';
	POST_Record[1].Name[7] = 'm';
	POST_Record[1].Name[8] = 'e';
	POST_Record[1].Name[9] = '_';
	POST_Record[1].Name[10] = 'b';
	POST_Record[1].NameLen = 11;
	POST_Record[1].FileIndex = 3;
	POST_Record[1].UpdateSelf = FALSE;
	POST_Record[1].PValue = &BUF_Text[1];
	pText = POST_Record[1].PValue;
	pText->CurrValue = postbuf_text1[0];
	pText->UserValue = postbuf_text1[1];
	pText->Offset = 1457;
	pText->DefaultLength = 13;
	pText->CurrValue[0] = 'b';
	pText->CurrValue[1] = 'l';
	pText->CurrValue[2] = '_';
	pText->CurrValue[3] = 'd';
	pText->CurrValue[4] = 'o';
	pText->CurrValue[5] = 'c';
	pText->CurrValue[6] = 'd';
	pText->CurrValue[7] = '2';
	pText->CurrValue[8] = '.';
	pText->CurrValue[9] = 'b';
	pText->CurrValue[10] = 'i';
	pText->CurrValue[11] = 'n';
	pText->CurrValue[12] = '"';
	pText->CurrLength = 13;

	/* for record 2 */
	POST_Record[2].Occupy = 1;
	POST_Record[2].PostType = POST_TYPE_SUBMIT;
	POST_Record[2].Name[0] = 'A';
	POST_Record[2].Name[1] = 'p';
	POST_Record[2].Name[2] = 'p';
	POST_Record[2].Name[3] = 'l';
	POST_Record[2].Name[4] = 'y';
	POST_Record[2].NameLen = 5;
	POST_Record[2].FileIndex = 0xff;
	POST_Record[2].UpdateSelf = FALSE;
	POST_Record[2].PValue = &BUF_Submit[0];
	pSubmit = POST_Record[2].PValue;
	pSubmit->Value[0] = 'A';
	pSubmit->Value[1] = 'p';
	pSubmit->Value[2] = 'p';
	pSubmit->Value[3] = 'l';
	pSubmit->Value[4] = 'y';
	pSubmit->DefaultVlaueLen = 5;
	pSubmit->IsApply = 1;

	/* for record 3 */
	POST_Record[3].Occupy = 1;
	POST_Record[3].PostType = POST_TYPE_SUBMIT;
	POST_Record[3].Name[0] = 'F';
	POST_Record[3].Name[1] = 'w';
	POST_Record[3].Name[2] = 'B';
	POST_Record[3].Name[3] = 'l';
	POST_Record[3].Name[4] = 'd';
	POST_Record[3].Name[5] = 'U';
	POST_Record[3].Name[6] = 'p';
	POST_Record[3].Name[7] = 'g';
	POST_Record[3].NameLen = 8;
	POST_Record[3].FileIndex = 0xff;
	POST_Record[3].UpdateSelf = FALSE;
	POST_Record[3].PValue = &BUF_Submit[1];
	pSubmit = POST_Record[3].PValue;
	pSubmit->Value[0] = 'U';
	pSubmit->Value[1] = 'p';
	pSubmit->Value[2] = 'g';
	pSubmit->Value[3] = 'r';
	pSubmit->Value[4] = 'a';
	pSubmit->Value[5] = 'd';
	pSubmit->Value[6] = 'e';
	pSubmit->Value[7] = '_';
	pSubmit->Value[8] = 'B';
	pSubmit->Value[9] = 'o';
	pSubmit->Value[10] = 'o';
	pSubmit->Value[11] = 't';
	pSubmit->Value[12] = 'l';
	pSubmit->Value[13] = 'd';
	pSubmit->Value[14] = 'r';
	pSubmit->DefaultVlaueLen = 15;
	pSubmit->IsApply = 1;

	/* for record 4 */
	POST_Record[4].Occupy = 1;
	POST_Record[4].PostType = POST_TYPE_TEXT;
	POST_Record[4].Name[0] = 't';
	POST_Record[4].Name[1] = 'f';
	POST_Record[4].Name[2] = 't';
	POST_Record[4].Name[3] = 'p';
	POST_Record[4].Name[4] = 's';
	POST_Record[4].Name[5] = '_';
	POST_Record[4].Name[6] = 'i';
	POST_Record[4].Name[7] = 'p';
	POST_Record[4].NameLen = 8;
	POST_Record[4].FileIndex = 3;
	POST_Record[4].UpdateSelf = FALSE;
	POST_Record[4].PValue = &BUF_Text[2];
	pText = POST_Record[4].PValue;
	pText->CurrValue = postbuf_text2[0];
	pText->UserValue = postbuf_text2[1];
	pText->Offset = 2070;
	pText->DefaultLength = 13;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '8';
	pText->CurrValue[11] = '0';
	pText->CurrValue[12] = '"';
	pText->CurrLength = 13;

	/* for record 5 */
	POST_Record[5].Occupy = 1;
	POST_Record[5].PostType = POST_TYPE_TEXT;
	POST_Record[5].Name[0] = 'f';
	POST_Record[5].Name[1] = 'i';
	POST_Record[5].Name[2] = 'l';
	POST_Record[5].Name[3] = 'e';
	POST_Record[5].Name[4] = '_';
	POST_Record[5].Name[5] = 'n';
	POST_Record[5].Name[6] = 'a';
	POST_Record[5].Name[7] = 'm';
	POST_Record[5].Name[8] = 'e';
	POST_Record[5].NameLen = 9;
	POST_Record[5].FileIndex = 3;
	POST_Record[5].UpdateSelf = FALSE;
	POST_Record[5].PValue = &BUF_Text[3];
	pText = POST_Record[5].PValue;
	pText->CurrValue = postbuf_text3[0];
	pText->UserValue = postbuf_text3[1];
	pText->Offset = 2189;
	pText->DefaultLength = 8;
	pText->CurrValue[0] = 'r';
	pText->CurrValue[1] = '2';
	pText->CurrValue[2] = 'w';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = 'b';
	pText->CurrValue[5] = 'i';
	pText->CurrValue[6] = 'n';
	pText->CurrValue[7] = '"';
	pText->CurrLength = 8;

	/* for record 6 */
	POST_Record[6].Occupy = 1;
	POST_Record[6].PostType = POST_TYPE_SUBMIT;
	POST_Record[6].Name[0] = 'u';
	POST_Record[6].Name[1] = 'p';
	POST_Record[6].Name[2] = 'g';
	POST_Record[6].Name[3] = 'r';
	POST_Record[6].Name[4] = 'a';
	POST_Record[6].Name[5] = 'd';
	POST_Record[6].Name[6] = 'e';
	POST_Record[6].NameLen = 7;
	POST_Record[6].FileIndex = 0xff;
	POST_Record[6].UpdateSelf = FALSE;
	POST_Record[6].PValue = &BUF_Submit[2];
	pSubmit = POST_Record[6].PValue;
	pSubmit->Value[0] = 'U';
	pSubmit->Value[1] = 'p';
	pSubmit->Value[2] = 'g';
	pSubmit->Value[3] = 'r';
	pSubmit->Value[4] = 'a';
	pSubmit->Value[5] = 'd';
	pSubmit->Value[6] = 'e';
	pSubmit->Value[7] = '_';
	pSubmit->Value[8] = 'M';
	pSubmit->Value[9] = 'C';
	pSubmit->Value[10] = 'P';
	pSubmit->Value[11] = 'U';
	pSubmit->DefaultVlaueLen = 12;
	pSubmit->IsApply = 1;

	/* for record 7 */
	POST_Record[7].Occupy = 1;
	POST_Record[7].PostType = POST_TYPE_TEXT;
	POST_Record[7].Name[0] = 't';
	POST_Record[7].Name[1] = 'f';
	POST_Record[7].Name[2] = 't';
	POST_Record[7].Name[3] = 'p';
	POST_Record[7].Name[4] = 's';
	POST_Record[7].Name[5] = '_';
	POST_Record[7].Name[6] = 'i';
	POST_Record[7].Name[7] = 'p';
	POST_Record[7].Name[8] = '_';
	POST_Record[7].Name[9] = 'w';
	POST_Record[7].NameLen = 10;
	POST_Record[7].FileIndex = 3;
	POST_Record[7].UpdateSelf = FALSE;
	POST_Record[7].PValue = &BUF_Text[4];
	pText = POST_Record[7].PValue;
	pText->CurrValue = postbuf_text4[0];
	pText->UserValue = postbuf_text4[1];
	pText->Offset = 2795;
	pText->DefaultLength = 13;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '8';
	pText->CurrValue[11] = '0';
	pText->CurrValue[12] = '"';
	pText->CurrLength = 13;

	/* for record 8 */
	POST_Record[8].Occupy = 1;
	POST_Record[8].PostType = POST_TYPE_TEXT;
	POST_Record[8].Name[0] = 'f';
	POST_Record[8].Name[1] = 'i';
	POST_Record[8].Name[2] = 'l';
	POST_Record[8].Name[3] = 'e';
	POST_Record[8].Name[4] = '_';
	POST_Record[8].Name[5] = 'n';
	POST_Record[8].Name[6] = 'a';
	POST_Record[8].Name[7] = 'm';
	POST_Record[8].Name[8] = 'e';
	POST_Record[8].Name[9] = '_';
	POST_Record[8].Name[10] = 'w';
	POST_Record[8].NameLen = 11;
	POST_Record[8].FileIndex = 3;
	POST_Record[8].UpdateSelf = FALSE;
	POST_Record[8].PValue = &BUF_Text[5];
	pText = POST_Record[8].PValue;
	pText->CurrValue = postbuf_text5[0];
	pText->UserValue = postbuf_text5[1];
	pText->Offset = 2916;
	pText->DefaultLength = 9;
	pText->CurrValue[0] = 'W';
	pText->CurrValue[1] = 'C';
	pText->CurrValue[2] = 'P';
	pText->CurrValue[3] = 'U';
	pText->CurrValue[4] = '.';
	pText->CurrValue[5] = 'b';
	pText->CurrValue[6] = 'i';
	pText->CurrValue[7] = 'n';
	pText->CurrValue[8] = '"';
	pText->CurrLength = 9;

	/* for record 9 */
	POST_Record[9].Occupy = 1;
	POST_Record[9].PostType = POST_TYPE_SUBMIT;
	POST_Record[9].Name[0] = 'F';
	POST_Record[9].Name[1] = 'w';
	POST_Record[9].Name[2] = 'W';
	POST_Record[9].Name[3] = 'U';
	POST_Record[9].Name[4] = 'p';
	POST_Record[9].Name[5] = 'g';
	POST_Record[9].NameLen = 6;
	POST_Record[9].FileIndex = 0xff;
	POST_Record[9].UpdateSelf = FALSE;
	POST_Record[9].PValue = &BUF_Submit[3];
	pSubmit = POST_Record[9].PValue;
	pSubmit->Value[0] = 'U';
	pSubmit->Value[1] = 'p';
	pSubmit->Value[2] = 'g';
	pSubmit->Value[3] = 'r';
	pSubmit->Value[4] = 'a';
	pSubmit->Value[5] = 'd';
	pSubmit->Value[6] = 'e';
	pSubmit->Value[7] = '_';
	pSubmit->Value[8] = 'W';
	pSubmit->Value[9] = 'C';
	pSubmit->Value[10] = 'P';
	pSubmit->Value[11] = 'U';
	pSubmit->DefaultVlaueLen = 12;
	pSubmit->IsApply = 1;

	/* for record 10 */
	POST_Record[10].Occupy = 1;
	POST_Record[10].PostType = POST_TYPE_TEXT;
	POST_Record[10].Name[0] = 'e';
	POST_Record[10].Name[1] = 'm';
	POST_Record[10].Name[2] = 'a';
	POST_Record[10].Name[3] = 'i';
	POST_Record[10].Name[4] = 'l';
	POST_Record[10].Name[5] = '_';
	POST_Record[10].Name[6] = 'a';
	POST_Record[10].Name[7] = 'd';
	POST_Record[10].Name[8] = 'd';
	POST_Record[10].Name[9] = 'r';
	POST_Record[10].NameLen = 10;
	POST_Record[10].FileIndex = 3;
	POST_Record[10].UpdateSelf = FALSE;
	POST_Record[10].PValue = &BUF_Text[6];
	pText = POST_Record[10].PValue;
	pText->CurrValue = postbuf_text6[0];
	pText->UserValue = postbuf_text6[1];
	pText->Offset = 3548;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 11 */
	POST_Record[11].Occupy = 1;
	POST_Record[11].PostType = POST_TYPE_TEXT;
	POST_Record[11].Name[0] = 'e';
	POST_Record[11].Name[1] = 'm';
	POST_Record[11].Name[2] = 'a';
	POST_Record[11].Name[3] = 'i';
	POST_Record[11].Name[4] = 'l';
	POST_Record[11].Name[5] = '_';
	POST_Record[11].Name[6] = 'f';
	POST_Record[11].Name[7] = 'r';
	POST_Record[11].Name[8] = 'o';
	POST_Record[11].Name[9] = 'm';
	POST_Record[11].NameLen = 10;
	POST_Record[11].FileIndex = 3;
	POST_Record[11].UpdateSelf = FALSE;
	POST_Record[11].PValue = &BUF_Text[7];
	pText = POST_Record[11].PValue;
	pText->CurrValue = postbuf_text7[0];
	pText->UserValue = postbuf_text7[1];
	pText->Offset = 3789;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 12 */
	POST_Record[12].Occupy = 1;
	POST_Record[12].PostType = POST_TYPE_TEXT;
	POST_Record[12].Name[0] = 'e';
	POST_Record[12].Name[1] = 'm';
	POST_Record[12].Name[2] = 'a';
	POST_Record[12].Name[3] = 'i';
	POST_Record[12].Name[4] = 'l';
	POST_Record[12].Name[5] = '_';
	POST_Record[12].Name[6] = 't';
	POST_Record[12].Name[7] = 'o';
	POST_Record[12].Name[8] = '1';
	POST_Record[12].NameLen = 9;
	POST_Record[12].FileIndex = 3;
	POST_Record[12].UpdateSelf = FALSE;
	POST_Record[12].PValue = &BUF_Text[8];
	pText = POST_Record[12].PValue;
	pText->CurrValue = postbuf_text8[0];
	pText->UserValue = postbuf_text8[1];
	pText->Offset = 3906;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 13 */
	POST_Record[13].Occupy = 1;
	POST_Record[13].PostType = POST_TYPE_TEXT;
	POST_Record[13].Name[0] = 'e';
	POST_Record[13].Name[1] = 'm';
	POST_Record[13].Name[2] = 'a';
	POST_Record[13].Name[3] = 'i';
	POST_Record[13].Name[4] = 'l';
	POST_Record[13].Name[5] = '_';
	POST_Record[13].Name[6] = 't';
	POST_Record[13].Name[7] = 'o';
	POST_Record[13].Name[8] = '2';
	POST_Record[13].NameLen = 9;
	POST_Record[13].FileIndex = 3;
	POST_Record[13].UpdateSelf = FALSE;
	POST_Record[13].PValue = &BUF_Text[9];
	pText = POST_Record[13].PValue;
	pText->CurrValue = postbuf_text9[0];
	pText->UserValue = postbuf_text9[1];
	pText->Offset = 4023;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 14 */
	POST_Record[14].Occupy = 1;
	POST_Record[14].PostType = POST_TYPE_TEXT;
	POST_Record[14].Name[0] = 'e';
	POST_Record[14].Name[1] = 'm';
	POST_Record[14].Name[2] = 'a';
	POST_Record[14].Name[3] = 'i';
	POST_Record[14].Name[4] = 'l';
	POST_Record[14].Name[5] = '_';
	POST_Record[14].Name[6] = 't';
	POST_Record[14].Name[7] = 'o';
	POST_Record[14].Name[8] = '3';
	POST_Record[14].NameLen = 9;
	POST_Record[14].FileIndex = 3;
	POST_Record[14].UpdateSelf = FALSE;
	POST_Record[14].PValue = &BUF_Text[10];
	pText = POST_Record[14].PValue;
	pText->CurrValue = postbuf_text10[0];
	pText->UserValue = postbuf_text10[1];
	pText->Offset = 4140;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 15 */
	POST_Record[15].Occupy = 1;
	POST_Record[15].PostType = POST_TYPE_SELECT;
	POST_Record[15].Name[0] = 'c';
	POST_Record[15].Name[1] = 'o';
	POST_Record[15].Name[2] = 'l';
	POST_Record[15].Name[3] = 'd';
	POST_Record[15].Name[4] = 's';
	POST_Record[15].Name[5] = 't';
	POST_Record[15].Name[6] = 'a';
	POST_Record[15].Name[7] = 'r';
	POST_Record[15].Name[8] = 't';
	POST_Record[15].NameLen = 9;
	POST_Record[15].FileIndex = 3;
	POST_Record[15].UpdateSelf = FALSE;
	POST_Record[15].PValue = &BUF_Select[0];
	pSelect = POST_Record[15].PValue;
	pSelect->Offset[0] = 4252;
	pSelect->Offset[1] = 4307;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 16 */
	POST_Record[16].Occupy = 1;
	POST_Record[16].PostType = POST_TYPE_SELECT;
	POST_Record[16].Name[0] = 'a';
	POST_Record[16].Name[1] = 'u';
	POST_Record[16].Name[2] = 't';
	POST_Record[16].Name[3] = 'h';
	POST_Record[16].Name[4] = 'f';
	POST_Record[16].Name[5] = 'a';
	POST_Record[16].Name[6] = 'i';
	POST_Record[16].Name[7] = 'l';
	POST_Record[16].NameLen = 8;
	POST_Record[16].FileIndex = 3;
	POST_Record[16].UpdateSelf = FALSE;
	POST_Record[16].PValue = &BUF_Select[1];
	pSelect = POST_Record[16].PValue;
	pSelect->Offset[0] = 4438;
	pSelect->Offset[1] = 4493;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 17 */
	POST_Record[17].Occupy = 1;
	POST_Record[17].PostType = POST_TYPE_SELECT;
	POST_Record[17].Name[0] = 'i';
	POST_Record[17].Name[1] = 'p';
	POST_Record[17].Name[2] = 'c';
	POST_Record[17].Name[3] = 'h';
	POST_Record[17].Name[4] = 'g';
	POST_Record[17].NameLen = 5;
	POST_Record[17].FileIndex = 3;
	POST_Record[17].UpdateSelf = FALSE;
	POST_Record[17].PValue = &BUF_Select[2];
	pSelect = POST_Record[17].PValue;
	pSelect->Offset[0] = 4623;
	pSelect->Offset[1] = 4678;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 18 */
	POST_Record[18].Occupy = 1;
	POST_Record[18].PostType = POST_TYPE_SELECT;
	POST_Record[18].Name[0] = 'p';
	POST_Record[18].Name[1] = 's';
	POST_Record[18].Name[2] = 'w';
	POST_Record[18].Name[3] = 'c';
	POST_Record[18].Name[4] = 'h';
	POST_Record[18].Name[5] = 'g';
	POST_Record[18].NameLen = 6;
	POST_Record[18].FileIndex = 3;
	POST_Record[18].UpdateSelf = FALSE;
	POST_Record[18].PValue = &BUF_Select[3];
	pSelect = POST_Record[18].PValue;
	pSelect->Offset[0] = 4801;
	pSelect->Offset[1] = 4856;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 19 */
	POST_Record[19].Occupy = 1;
	POST_Record[19].PostType = POST_TYPE_RADIO;
	POST_Record[19].Name[0] = 'd';
	POST_Record[19].Name[1] = 'h';
	POST_Record[19].Name[2] = 'c';
	POST_Record[19].Name[3] = 'p';
	POST_Record[19].Name[4] = 's';
	POST_Record[19].Name[5] = '_';
	POST_Record[19].Name[6] = 'o';
	POST_Record[19].Name[7] = 'n';
	POST_Record[19].Name[8] = 'o';
	POST_Record[19].Name[9] = 'f';
	POST_Record[19].Name[10] = 'f';
	POST_Record[19].NameLen = 11;
	POST_Record[19].FileIndex = 3;
	POST_Record[19].UpdateSelf = FALSE;
	POST_Record[19].PValue = &BUF_Radio[0];
	pRadio = POST_Record[19].PValue;
	pRadio->Offset[0] = 5280;
	pRadio->Value[0][0] = '0';
	pRadio->Length[0] = 1;
	pRadio->Offset[1] = 5358;
	pRadio->Value[1][0] = '1';
	pRadio->Length[1] = 1;
	pRadio->Count = 2;
	pRadio->DefaultSet = 1;
	pRadio->CurrentSet = 1;
	pRadio->UserSet = 1;

	/* for record 20 */
	POST_Record[20].Occupy = 1;
	POST_Record[20].PostType = POST_TYPE_TEXT;
	POST_Record[20].Name[0] = 'd';
	POST_Record[20].Name[1] = 'h';
	POST_Record[20].Name[2] = 'c';
	POST_Record[20].Name[3] = 'p';
	POST_Record[20].Name[4] = 's';
	POST_Record[20].Name[5] = '_';
	POST_Record[20].Name[6] = 's';
	POST_Record[20].Name[7] = 't';
	POST_Record[20].Name[8] = 'a';
	POST_Record[20].Name[9] = 'r';
	POST_Record[20].Name[10] = 't';
	POST_Record[20].Name[11] = 'i';
	POST_Record[20].Name[12] = 'p';
	POST_Record[20].NameLen = 13;
	POST_Record[20].FileIndex = 3;
	POST_Record[20].UpdateSelf = FALSE;
	POST_Record[20].PValue = &BUF_Text[11];
	pText = POST_Record[20].PValue;
	pText->CurrValue = postbuf_text11[0];
	pText->UserValue = postbuf_text11[1];
	pText->Offset = 5481;
	pText->DefaultLength = 12;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '2';
	pText->CurrValue[11] = '"';
	pText->CurrLength = 12;

	/* for record 21 */
	POST_Record[21].Occupy = 1;
	POST_Record[21].PostType = POST_TYPE_TEXT;
	POST_Record[21].Name[0] = 'd';
	POST_Record[21].Name[1] = 'h';
	POST_Record[21].Name[2] = 'c';
	POST_Record[21].Name[3] = 'p';
	POST_Record[21].Name[4] = 's';
	POST_Record[21].Name[5] = '_';
	POST_Record[21].Name[6] = 'e';
	POST_Record[21].Name[7] = 'n';
	POST_Record[21].Name[8] = 'd';
	POST_Record[21].Name[9] = 'i';
	POST_Record[21].Name[10] = 'p';
	POST_Record[21].NameLen = 11;
	POST_Record[21].FileIndex = 3;
	POST_Record[21].UpdateSelf = FALSE;
	POST_Record[21].PValue = &BUF_Text[12];
	pText = POST_Record[21].PValue;
	pText->CurrValue = postbuf_text12[0];
	pText->UserValue = postbuf_text12[1];
	pText->Offset = 5614;
	pText->DefaultLength = 14;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '2';
	pText->CurrValue[11] = '5';
	pText->CurrValue[12] = '4';
	pText->CurrValue[13] = '"';
	pText->CurrLength = 14;

	/* for record 22 */
	POST_Record[22].Occupy = 1;
	POST_Record[22].PostType = POST_TYPE_TEXT;
	POST_Record[22].Name[0] = 'd';
	POST_Record[22].Name[1] = 'h';
	POST_Record[22].Name[2] = 'c';
	POST_Record[22].Name[3] = 'p';
	POST_Record[22].Name[4] = 's';
	POST_Record[22].Name[5] = '_';
	POST_Record[22].Name[6] = 'g';
	POST_Record[22].Name[7] = 'a';
	POST_Record[22].Name[8] = 't';
	POST_Record[22].Name[9] = 'e';
	POST_Record[22].Name[10] = 'w';
	POST_Record[22].Name[11] = 'a';
	POST_Record[22].Name[12] = 'y';
	POST_Record[22].NameLen = 13;
	POST_Record[22].FileIndex = 3;
	POST_Record[22].UpdateSelf = FALSE;
	POST_Record[22].PValue = &BUF_Text[13];
	pText = POST_Record[22].PValue;
	pText->CurrValue = postbuf_text13[0];
	pText->UserValue = postbuf_text13[1];
	pText->Offset = 5744;
	pText->DefaultLength = 12;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '1';
	pText->CurrValue[11] = '"';
	pText->CurrLength = 12;

	/* for record 23 */
	POST_Record[23].Occupy = 1;
	POST_Record[23].PostType = POST_TYPE_TEXT;
	POST_Record[23].Name[0] = 'd';
	POST_Record[23].Name[1] = 'h';
	POST_Record[23].Name[2] = 'c';
	POST_Record[23].Name[3] = 'p';
	POST_Record[23].Name[4] = 's';
	POST_Record[23].Name[5] = '_';
	POST_Record[23].Name[6] = 'm';
	POST_Record[23].Name[7] = 'a';
	POST_Record[23].Name[8] = 's';
	POST_Record[23].Name[9] = 'k';
	POST_Record[23].NameLen = 10;
	POST_Record[23].FileIndex = 3;
	POST_Record[23].UpdateSelf = FALSE;
	POST_Record[23].PValue = &BUF_Text[14];
	pText = POST_Record[23].PValue;
	pText->CurrValue = postbuf_text14[0];
	pText->UserValue = postbuf_text14[1];
	pText->Offset = 5865;
	pText->DefaultLength = 14;
	pText->CurrValue[0] = '2';
	pText->CurrValue[1] = '5';
	pText->CurrValue[2] = '5';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '2';
	pText->CurrValue[5] = '5';
	pText->CurrValue[6] = '5';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '2';
	pText->CurrValue[9] = '5';
	pText->CurrValue[10] = '5';
	pText->CurrValue[11] = '.';
	pText->CurrValue[12] = '0';
	pText->CurrValue[13] = '"';
	pText->CurrLength = 14;

	/* for record 24 */
	POST_Record[24].Occupy = 1;
	POST_Record[24].PostType = POST_TYPE_TEXT;
	POST_Record[24].Name[0] = 'd';
	POST_Record[24].Name[1] = 's';
	POST_Record[24].Name[2] = 'm';
	POST_Record[24].Name[3] = '_';
	POST_Record[24].Name[4] = 'n';
	POST_Record[24].Name[5] = 'a';
	POST_Record[24].Name[6] = 'm';
	POST_Record[24].Name[7] = 'e';
	POST_Record[24].NameLen = 8;
	POST_Record[24].FileIndex = 7;
	POST_Record[24].UpdateSelf = FALSE;
	POST_Record[24].PValue = &BUF_Text[15];
	pText = POST_Record[24].PValue;
	pText->CurrValue = postbuf_text15[0];
	pText->UserValue = postbuf_text15[1];
	pText->Offset = 1315;
	pText->DefaultLength = 5;
	pText->CurrValue[0] = 'D';
	pText->CurrValue[1] = 'S';
	pText->CurrValue[2] = 'M';
	pText->CurrValue[3] = '1';
	pText->CurrValue[4] = '"';
	pText->CurrLength = 5;

	/* for record 25 */
	POST_Record[25].Occupy = 1;
	POST_Record[25].PostType = POST_TYPE_SELECT;
	POST_Record[25].Name[0] = 'd';
	POST_Record[25].Name[1] = 'b';
	POST_Record[25].Name[2] = 'r';
	POST_Record[25].NameLen = 3;
	POST_Record[25].FileIndex = 7;
	POST_Record[25].UpdateSelf = FALSE;
	POST_Record[25].PValue = &BUF_Select[4];
	pSelect = POST_Record[25].PValue;
	pSelect->Offset[0] = 1522;
	pSelect->Offset[1] = 1576;
	pSelect->Offset[2] = 1621;
	pSelect->Offset[3] = 1665;
	pSelect->Offset[4] = 1709;
	pSelect->Offset[5] = 1753;
	pSelect->Offset[6] = 1796;
	pSelect->Offset[7] = 1839;
	pSelect->Offset[8] = 1882;
	pSelect->Count = 9;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 26 */
	POST_Record[26].Occupy = 1;
	POST_Record[26].PostType = POST_TYPE_SELECT;
	POST_Record[26].Name[0] = 'd';
	POST_Record[26].Name[1] = 'a';
	POST_Record[26].Name[2] = 't';
	POST_Record[26].Name[3] = 'a';
	POST_Record[26].NameLen = 4;
	POST_Record[26].FileIndex = 7;
	POST_Record[26].UpdateSelf = FALSE;
	POST_Record[26].PValue = &BUF_Select[5];
	pSelect = POST_Record[26].PValue;
	pSelect->Offset[0] = 1994;
	pSelect->Offset[1] = 2043;
	pSelect->Offset[2] = 2083;
	pSelect->Offset[3] = 2123;
	pSelect->Count = 4;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 27 */
	POST_Record[27].Occupy = 1;
	POST_Record[27].PostType = POST_TYPE_SELECT;
	POST_Record[27].Name[0] = 'p';
	POST_Record[27].Name[1] = 'a';
	POST_Record[27].Name[2] = 'r';
	POST_Record[27].Name[3] = 'i';
	POST_Record[27].Name[4] = 't';
	POST_Record[27].Name[5] = 'y';
	POST_Record[27].NameLen = 6;
	POST_Record[27].FileIndex = 7;
	POST_Record[27].UpdateSelf = FALSE;
	POST_Record[27].PValue = &BUF_Select[6];
	pSelect = POST_Record[27].PValue;
	pSelect->Offset[0] = 2236;
	pSelect->Offset[1] = 2287;
	pSelect->Offset[2] = 2330;
	pSelect->Count = 3;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 28 */
	POST_Record[28].Occupy = 1;
	POST_Record[28].PostType = POST_TYPE_SELECT;
	POST_Record[28].Name[0] = 's';
	POST_Record[28].Name[1] = 't';
	POST_Record[28].Name[2] = 'o';
	POST_Record[28].Name[3] = 'p';
	POST_Record[28].NameLen = 4;
	POST_Record[28].FileIndex = 7;
	POST_Record[28].UpdateSelf = FALSE;
	POST_Record[28].PValue = &BUF_Select[7];
	pSelect = POST_Record[28].PValue;
	pSelect->Offset[0] = 2442;
	pSelect->Offset[1] = 2491;
	pSelect->Offset[2] = 2533;
	pSelect->Count = 3;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 29 */
	POST_Record[29].Occupy = 1;
	POST_Record[29].PostType = POST_TYPE_SELECT;
	POST_Record[29].Name[0] = 'f';
	POST_Record[29].Name[1] = 'l';
	POST_Record[29].Name[2] = 'o';
	POST_Record[29].Name[3] = 'w';
	POST_Record[29].NameLen = 4;
	POST_Record[29].FileIndex = 7;
	POST_Record[29].UpdateSelf = FALSE;
	POST_Record[29].PValue = &BUF_Select[8];
	pSelect = POST_Record[29].PValue;
	pSelect->Offset[0] = 2645;
	pSelect->Offset[1] = 2701;
	pSelect->Offset[2] = 2748;
	pSelect->Count = 3;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 30 */
	POST_Record[30].Occupy = 1;
	POST_Record[30].PostType = POST_TYPE_SELECT;
	POST_Record[30].Name[0] = 'r';
	POST_Record[30].Name[1] = 's';
	POST_Record[30].Name[2] = '4';
	POST_Record[30].Name[3] = '8';
	POST_Record[30].Name[4] = '5';
	POST_Record[30].NameLen = 5;
	POST_Record[30].FileIndex = 7;
	POST_Record[30].UpdateSelf = FALSE;
	POST_Record[30].PValue = &BUF_Select[9];
	pSelect = POST_Record[30].PValue;
	pSelect->Offset[0] = 2877;
	pSelect->Offset[1] = 2930;
	pSelect->Offset[2] = 2991;
	pSelect->Offset[3] = 3060;
	pSelect->Count = 4;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 31 */
	POST_Record[31].Occupy = 1;
	POST_Record[31].PostType = POST_TYPE_SELECT;
	POST_Record[31].Name[0] = 'd';
	POST_Record[31].Name[1] = 'h';
	POST_Record[31].Name[2] = 'c';
	POST_Record[31].Name[3] = 'p';
	POST_Record[31].NameLen = 4;
	POST_Record[31].FileIndex = 7;
	POST_Record[31].UpdateSelf = FALSE;
	POST_Record[31].PValue = &BUF_Select[10];
	pSelect = POST_Record[31].PValue;
	pSelect->Offset[0] = 3248;
	pSelect->Offset[1] = 3303;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 32 */
	POST_Record[32].Occupy = 1;
	POST_Record[32].PostType = POST_TYPE_TEXT;
	POST_Record[32].Name[0] = 's';
	POST_Record[32].Name[1] = 't';
	POST_Record[32].Name[2] = 'a';
	POST_Record[32].Name[3] = 't';
	POST_Record[32].Name[4] = 'i';
	POST_Record[32].Name[5] = 'c';
	POST_Record[32].Name[6] = '_';
	POST_Record[32].Name[7] = 'i';
	POST_Record[32].Name[8] = 'p';
	POST_Record[32].NameLen = 9;
	POST_Record[32].FileIndex = 7;
	POST_Record[32].UpdateSelf = FALSE;
	POST_Record[32].PValue = &BUF_Text[16];
	pText = POST_Record[32].PValue;
	pText->CurrValue = postbuf_text16[0];
	pText->UserValue = postbuf_text16[1];
	pText->Offset = 3426;
	pText->DefaultLength = 12;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '3';
	pText->CurrValue[11] = '"';
	pText->CurrLength = 12;

	/* for record 33 */
	POST_Record[33].Occupy = 1;
	POST_Record[33].PostType = POST_TYPE_TEXT;
	POST_Record[33].Name[0] = 'm';
	POST_Record[33].Name[1] = 'a';
	POST_Record[33].Name[2] = 's';
	POST_Record[33].Name[3] = 'k';
	POST_Record[33].NameLen = 4;
	POST_Record[33].FileIndex = 7;
	POST_Record[33].UpdateSelf = FALSE;
	POST_Record[33].PValue = &BUF_Text[17];
	pText = POST_Record[33].PValue;
	pText->CurrValue = postbuf_text17[0];
	pText->UserValue = postbuf_text17[1];
	pText->Offset = 3548;
	pText->DefaultLength = 14;
	pText->CurrValue[0] = '2';
	pText->CurrValue[1] = '5';
	pText->CurrValue[2] = '5';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '2';
	pText->CurrValue[5] = '5';
	pText->CurrValue[6] = '5';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '2';
	pText->CurrValue[9] = '5';
	pText->CurrValue[10] = '5';
	pText->CurrValue[11] = '.';
	pText->CurrValue[12] = '0';
	pText->CurrValue[13] = '"';
	pText->CurrLength = 14;

	/* for record 34 */
	POST_Record[34].Occupy = 1;
	POST_Record[34].PostType = POST_TYPE_TEXT;
	POST_Record[34].Name[0] = 'g';
	POST_Record[34].Name[1] = 'a';
	POST_Record[34].Name[2] = 't';
	POST_Record[34].Name[3] = 'e';
	POST_Record[34].Name[4] = 'w';
	POST_Record[34].Name[5] = 'a';
	POST_Record[34].Name[6] = 'y';
	POST_Record[34].Name[7] = '_';
	POST_Record[34].Name[8] = 'i';
	POST_Record[34].Name[9] = 'p';
	POST_Record[34].NameLen = 10;
	POST_Record[34].FileIndex = 7;
	POST_Record[34].UpdateSelf = FALSE;
	POST_Record[34].PValue = &BUF_Text[18];
	pText = POST_Record[34].PValue;
	pText->CurrValue = postbuf_text18[0];
	pText->UserValue = postbuf_text18[1];
	pText->Offset = 3682;
	pText->DefaultLength = 12;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '9';
	pText->CurrValue[2] = '2';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '1';
	pText->CurrValue[5] = '6';
	pText->CurrValue[6] = '8';
	pText->CurrValue[7] = '.';
	pText->CurrValue[8] = '0';
	pText->CurrValue[9] = '.';
	pText->CurrValue[10] = '1';
	pText->CurrValue[11] = '"';
	pText->CurrLength = 12;

	/* for record 35 */
	POST_Record[35].Occupy = 1;
	POST_Record[35].PostType = POST_TYPE_TEXT;
	POST_Record[35].Name[0] = 'd';
	POST_Record[35].Name[1] = 'n';
	POST_Record[35].Name[2] = 's';
	POST_Record[35].Name[3] = '_';
	POST_Record[35].Name[4] = 'i';
	POST_Record[35].Name[5] = 'p';
	POST_Record[35].NameLen = 6;
	POST_Record[35].FileIndex = 7;
	POST_Record[35].UpdateSelf = FALSE;
	POST_Record[35].PValue = &BUF_Text[19];
	pText = POST_Record[35].PValue;
	pText->CurrValue = postbuf_text19[0];
	pText->UserValue = postbuf_text19[1];
	pText->Offset = 3805;
	pText->DefaultLength = 11;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '6';
	pText->CurrValue[2] = '8';
	pText->CurrValue[3] = '.';
	pText->CurrValue[4] = '9';
	pText->CurrValue[5] = '5';
	pText->CurrValue[6] = '.';
	pText->CurrValue[7] = '1';
	pText->CurrValue[8] = '.';
	pText->CurrValue[9] = '1';
	pText->CurrValue[10] = '"';
	pText->CurrLength = 11;

	/* for record 36 */
	POST_Record[36].Occupy = 1;
	POST_Record[36].PostType = POST_TYPE_SELECT;
	POST_Record[36].Name[0] = 'c';
	POST_Record[36].Name[1] = 'o';
	POST_Record[36].Name[2] = 'n';
	POST_Record[36].Name[3] = 'n';
	POST_Record[36].Name[4] = 't';
	POST_Record[36].Name[5] = 'y';
	POST_Record[36].Name[6] = 'p';
	POST_Record[36].Name[7] = 'e';
	POST_Record[36].NameLen = 8;
	POST_Record[36].FileIndex = 7;
	POST_Record[36].UpdateSelf = FALSE;
	POST_Record[36].PValue = &BUF_Select[11];
	pSelect = POST_Record[36].PValue;
	pSelect->Offset[0] = 3931;
	pSelect->Offset[1] = 3982;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 37 */
	POST_Record[37].Occupy = 1;
	POST_Record[37].PostType = POST_TYPE_TEXT;
	POST_Record[37].Name[0] = 't';
	POST_Record[37].Name[1] = 'x';
	POST_Record[37].Name[2] = 't';
	POST_Record[37].Name[3] = 'i';
	POST_Record[37].Name[4] = 'm';
	POST_Record[37].Name[5] = 'e';
	POST_Record[37].Name[6] = 'r';
	POST_Record[37].NameLen = 7;
	POST_Record[37].FileIndex = 7;
	POST_Record[37].UpdateSelf = FALSE;
	POST_Record[37].PValue = &BUF_Text[20];
	pText = POST_Record[37].PValue;
	pText->CurrValue = postbuf_text20[0];
	pText->UserValue = postbuf_text20[1];
	pText->Offset = 4102;
	pText->DefaultLength = 4;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '0';
	pText->CurrValue[2] = '0';
	pText->CurrValue[3] = '"';
	pText->CurrLength = 4;

	/* for record 38 */
	POST_Record[38].Occupy = 1;
	POST_Record[38].PostType = POST_TYPE_SELECT;
	POST_Record[38].Name[0] = 'c';
	POST_Record[38].Name[1] = 's';
	POST_Record[38].Name[2] = '_';
	POST_Record[38].Name[3] = 'm';
	POST_Record[38].Name[4] = 'o';
	POST_Record[38].Name[5] = 'd';
	POST_Record[38].Name[6] = 'e';
	POST_Record[38].NameLen = 7;
	POST_Record[38].FileIndex = 7;
	POST_Record[38].UpdateSelf = FALSE;
	POST_Record[38].PValue = &BUF_Select[12];
	pSelect = POST_Record[38].PValue;
	pSelect->Offset[0] = 4317;
	pSelect->Offset[1] = 4371;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 39 */
	POST_Record[39].Occupy = 1;
	POST_Record[39].PostType = POST_TYPE_TEXT;
	POST_Record[39].Name[0] = 's';
	POST_Record[39].Name[1] = '_';
	POST_Record[39].Name[2] = 'l';
	POST_Record[39].Name[3] = 's';
	POST_Record[39].Name[4] = 't';
	POST_Record[39].Name[5] = 'p';
	POST_Record[39].Name[6] = 'o';
	POST_Record[39].Name[7] = 'r';
	POST_Record[39].Name[8] = 't';
	POST_Record[39].NameLen = 9;
	POST_Record[39].FileIndex = 7;
	POST_Record[39].UpdateSelf = FALSE;
	POST_Record[39].PValue = &BUF_Text[21];
	pText = POST_Record[39].PValue;
	pText->CurrValue = postbuf_text21[0];
	pText->UserValue = postbuf_text21[1];
	pText->Offset = 4498;
	pText->DefaultLength = 5;
	pText->CurrValue[0] = '5';
	pText->CurrValue[1] = '0';
	pText->CurrValue[2] = '0';
	pText->CurrValue[3] = '0';
	pText->CurrValue[4] = '"';
	pText->CurrLength = 5;

	/* for record 40 */
	POST_Record[40].Occupy = 1;
	POST_Record[40].PostType = POST_TYPE_TEXT;
	POST_Record[40].Name[0] = 'c';
	POST_Record[40].Name[1] = '_';
	POST_Record[40].Name[2] = 'd';
	POST_Record[40].Name[3] = 'e';
	POST_Record[40].Name[4] = 's';
	POST_Record[40].Name[5] = 'h';
	POST_Record[40].Name[6] = 'n';
	POST_Record[40].NameLen = 7;
	POST_Record[40].FileIndex = 7;
	POST_Record[40].UpdateSelf = FALSE;
	POST_Record[40].PValue = &BUF_Text[22];
	pText = POST_Record[40].PValue;
	pText->CurrValue = postbuf_text22[0];
	pText->UserValue = postbuf_text22[1];
	pText->Offset = 4725;
	pText->DefaultLength = 12;
	pText->CurrValue[0] = 'a';
	pText->CurrValue[1] = 's';
	pText->CurrValue[2] = 'i';
	pText->CurrValue[3] = 'x';
	pText->CurrValue[4] = '.';
	pText->CurrValue[5] = 'c';
	pText->CurrValue[6] = 'o';
	pText->CurrValue[7] = 'm';
	pText->CurrValue[8] = '.';
	pText->CurrValue[9] = 't';
	pText->CurrValue[10] = 'w';
	pText->CurrValue[11] = '"';
	pText->CurrLength = 12;

	/* for record 41 */
	POST_Record[41].Occupy = 1;
	POST_Record[41].PostType = POST_TYPE_TEXT;
	POST_Record[41].Name[0] = 'c';
	POST_Record[41].Name[1] = '_';
	POST_Record[41].Name[2] = 'd';
	POST_Record[41].Name[3] = 'e';
	POST_Record[41].Name[4] = 's';
	POST_Record[41].Name[5] = 'p';
	POST_Record[41].Name[6] = 'o';
	POST_Record[41].Name[7] = 'r';
	POST_Record[41].Name[8] = 't';
	POST_Record[41].NameLen = 9;
	POST_Record[41].FileIndex = 7;
	POST_Record[41].UpdateSelf = FALSE;
	POST_Record[41].PValue = &BUF_Text[23];
	pText = POST_Record[41].PValue;
	pText->CurrValue = postbuf_text23[0];
	pText->UserValue = postbuf_text23[1];
	pText->Offset = 4980;
	pText->DefaultLength = 5;
	pText->CurrValue[0] = '5';
	pText->CurrValue[1] = '0';
	pText->CurrValue[2] = '0';
	pText->CurrValue[3] = '0';
	pText->CurrValue[4] = '"';
	pText->CurrLength = 5;

	/* for record 42 */
	POST_Record[42].Occupy = 1;
	POST_Record[42].PostType = POST_TYPE_SUBMIT;
	POST_Record[42].Name[0] = 'r';
	POST_Record[42].Name[1] = 'e';
	POST_Record[42].Name[2] = 's';
	POST_Record[42].Name[3] = 't';
	POST_Record[42].Name[4] = 'o';
	POST_Record[42].Name[5] = 'r';
	POST_Record[42].Name[6] = 'e';
	POST_Record[42].NameLen = 7;
	POST_Record[42].FileIndex = 0xff;
	POST_Record[42].UpdateSelf = FALSE;
	POST_Record[42].PValue = &BUF_Submit[4];
	pSubmit = POST_Record[42].PValue;
	pSubmit->Value[0] = 'R';
	pSubmit->Value[1] = 'e';
	pSubmit->Value[2] = 's';
	pSubmit->Value[3] = 't';
	pSubmit->Value[4] = 'o';
	pSubmit->Value[5] = 'r';
	pSubmit->Value[6] = 'e';
	pSubmit->Value[7] = '_';
	pSubmit->Value[8] = 'D';
	pSubmit->Value[9] = 'e';
	pSubmit->Value[10] = 'f';
	pSubmit->Value[11] = 'a';
	pSubmit->Value[12] = 'u';
	pSubmit->Value[13] = 'l';
	pSubmit->Value[14] = 't';
	pSubmit->DefaultVlaueLen = 15;
	pSubmit->IsApply = 1;

	/* for record 43 */
	POST_Record[43].Occupy = 1;
	POST_Record[43].PostType = POST_TYPE_SUBMIT;
	POST_Record[43].Name[0] = 'r';
	POST_Record[43].Name[1] = 'e';
	POST_Record[43].Name[2] = 'b';
	POST_Record[43].Name[3] = 'o';
	POST_Record[43].Name[4] = 'o';
	POST_Record[43].Name[5] = 't';
	POST_Record[43].NameLen = 6;
	POST_Record[43].FileIndex = 0xff;
	POST_Record[43].UpdateSelf = FALSE;
	POST_Record[43].PValue = &BUF_Submit[5];
	pSubmit = POST_Record[43].PValue;
	pSubmit->Value[0] = 'R';
	pSubmit->Value[1] = 'e';
	pSubmit->Value[2] = 'b';
	pSubmit->Value[3] = 'o';
	pSubmit->Value[4] = 'o';
	pSubmit->Value[5] = 't';
	pSubmit->DefaultVlaueLen = 6;
	pSubmit->IsApply = 1;

	/* for record 44 */
	POST_Record[44].Occupy = 1;
	POST_Record[44].PostType = POST_TYPE_TEXT;
	POST_Record[44].Name[0] = 'c';
	POST_Record[44].Name[1] = 'm';
	POST_Record[44].Name[2] = 'd';
	POST_Record[44].NameLen = 3;
	POST_Record[44].FileIndex = 8;
	POST_Record[44].UpdateSelf = FALSE;
	POST_Record[44].PValue = &BUF_Text[24];
	pText = POST_Record[44].PValue;
	pText->CurrValue = postbuf_text24[0];
	pText->UserValue = postbuf_text24[1];
	pText->Offset = 326;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 45 */
	POST_Record[45].Occupy = 1;
	POST_Record[45].PostType = POST_TYPE_TEXT;
	POST_Record[45].Name[0] = 'u';
	POST_Record[45].Name[1] = 's';
	POST_Record[45].Name[2] = 'e';
	POST_Record[45].Name[3] = 'r';
	POST_Record[45].Name[4] = 'n';
	POST_Record[45].Name[5] = 'a';
	POST_Record[45].Name[6] = 'm';
	POST_Record[45].Name[7] = 'e';
	POST_Record[45].NameLen = 8;
	POST_Record[45].FileIndex = 9;
	POST_Record[45].UpdateSelf = FALSE;
	POST_Record[45].PValue = &BUF_Text[25];
	pText = POST_Record[45].PValue;
	pText->CurrValue = postbuf_text25[0];
	pText->UserValue = postbuf_text25[1];
	pText->Offset = 1001;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 46 */
	POST_Record[46].Occupy = 1;
	POST_Record[46].PostType = POST_TYPE_PASSWORD;
	POST_Record[46].Name[0] = 'p';
	POST_Record[46].Name[1] = 'a';
	POST_Record[46].Name[2] = 's';
	POST_Record[46].Name[3] = 's';
	POST_Record[46].Name[4] = 'w';
	POST_Record[46].Name[5] = 'o';
	POST_Record[46].Name[6] = 'r';
	POST_Record[46].Name[7] = 'd';
	POST_Record[46].NameLen = 8;
	POST_Record[46].FileIndex = 9;
	POST_Record[46].UpdateSelf = FALSE;
	POST_Record[46].PValue = &BUF_Password[0];
	pPassword = POST_Record[46].PValue;
	pPassword->CurrValue = postbuf_password0[0];
	pPassword->UserValue = postbuf_password0[1];
	pPassword->Offset = 1110;
	pPassword->DefaultLength = 1;
	pPassword->CurrValue[0] = '"';
	pPassword->CurrLength = 1;

	/* for record 47 */
	POST_Record[47].Occupy = 1;
	POST_Record[47].PostType = POST_TYPE_SUBMIT;
	POST_Record[47].Name[0] = 'L';
	POST_Record[47].Name[1] = 'o';
	POST_Record[47].Name[2] = 'g';
	POST_Record[47].Name[3] = 'i';
	POST_Record[47].Name[4] = 'n';
	POST_Record[47].NameLen = 5;
	POST_Record[47].FileIndex = 0xff;
	POST_Record[47].UpdateSelf = FALSE;
	POST_Record[47].PValue = &BUF_Submit[6];
	pSubmit = POST_Record[47].PValue;
	pSubmit->Value[0] = 'L';
	pSubmit->Value[1] = 'o';
	pSubmit->Value[2] = 'g';
	pSubmit->Value[3] = 'i';
	pSubmit->Value[4] = 'n';
	pSubmit->DefaultVlaueLen = 5;
	pSubmit->IsApply = 1;

	/* for record 48 */
	POST_Record[48].Occupy = 1;
	POST_Record[48].PostType = POST_TYPE_TEXT;
	POST_Record[48].Name[0] = 'n';
	POST_Record[48].Name[1] = 'e';
	POST_Record[48].Name[2] = 'w';
	POST_Record[48].Name[3] = '_';
	POST_Record[48].Name[4] = 'u';
	POST_Record[48].Name[5] = 's';
	POST_Record[48].Name[6] = 'n';
	POST_Record[48].NameLen = 7;
	POST_Record[48].FileIndex = 10;
	POST_Record[48].UpdateSelf = FALSE;
	POST_Record[48].PValue = &BUF_Text[26];
	pText = POST_Record[48].PValue;
	pText->CurrValue = postbuf_text26[0];
	pText->UserValue = postbuf_text26[1];
	pText->Offset = 1328;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 49 */
	POST_Record[49].Occupy = 1;
	POST_Record[49].PostType = POST_TYPE_PASSWORD;
	POST_Record[49].Name[0] = 'o';
	POST_Record[49].Name[1] = 'l';
	POST_Record[49].Name[2] = 'd';
	POST_Record[49].Name[3] = '_';
	POST_Record[49].Name[4] = 'p';
	POST_Record[49].Name[5] = 's';
	POST_Record[49].Name[6] = 'w';
	POST_Record[49].NameLen = 7;
	POST_Record[49].FileIndex = 10;
	POST_Record[49].UpdateSelf = FALSE;
	POST_Record[49].PValue = &BUF_Password[1];
	pPassword = POST_Record[49].PValue;
	pPassword->CurrValue = postbuf_password1[0];
	pPassword->UserValue = postbuf_password1[1];
	pPassword->Offset = 1740;
	pPassword->DefaultLength = 1;
	pPassword->CurrValue[0] = '"';
	pPassword->CurrLength = 1;

	/* for record 50 */
	POST_Record[50].Occupy = 1;
	POST_Record[50].PostType = POST_TYPE_PASSWORD;
	POST_Record[50].Name[0] = 'n';
	POST_Record[50].Name[1] = 'e';
	POST_Record[50].Name[2] = 'w';
	POST_Record[50].Name[3] = '_';
	POST_Record[50].Name[4] = 'p';
	POST_Record[50].Name[5] = 's';
	POST_Record[50].Name[6] = 'w';
	POST_Record[50].NameLen = 7;
	POST_Record[50].FileIndex = 10;
	POST_Record[50].UpdateSelf = FALSE;
	POST_Record[50].PValue = &BUF_Password[2];
	pPassword = POST_Record[50].PValue;
	pPassword->CurrValue = postbuf_password2[0];
	pPassword->UserValue = postbuf_password2[1];
	pPassword->Offset = 1852;
	pPassword->DefaultLength = 1;
	pPassword->CurrValue[0] = '"';
	pPassword->CurrLength = 1;

	/* for record 51 */
	POST_Record[51].Occupy = 1;
	POST_Record[51].PostType = POST_TYPE_PASSWORD;
	POST_Record[51].Name[0] = 'c';
	POST_Record[51].Name[1] = 'f';
	POST_Record[51].Name[2] = 'm';
	POST_Record[51].Name[3] = '_';
	POST_Record[51].Name[4] = 'p';
	POST_Record[51].Name[5] = 's';
	POST_Record[51].Name[6] = 'w';
	POST_Record[51].NameLen = 7;
	POST_Record[51].FileIndex = 10;
	POST_Record[51].UpdateSelf = FALSE;
	POST_Record[51].PValue = &BUF_Password[3];
	pPassword = POST_Record[51].PValue;
	pPassword->CurrValue = postbuf_password3[0];
	pPassword->UserValue = postbuf_password3[1];
	pPassword->Offset = 1968;
	pPassword->DefaultLength = 1;
	pPassword->CurrValue[0] = '"';
	pPassword->CurrLength = 1;

	/* for record 52 */
	POST_Record[52].Occupy = 1;
	POST_Record[52].PostType = POST_TYPE_TEXT;
	POST_Record[52].Name[0] = 'a';
	POST_Record[52].Name[1] = 'c';
	POST_Record[52].Name[2] = 'c';
	POST_Record[52].Name[3] = '_';
	POST_Record[52].Name[4] = 'i';
	POST_Record[52].Name[5] = 'p';
	POST_Record[52].Name[6] = '1';
	POST_Record[52].NameLen = 7;
	POST_Record[52].FileIndex = 10;
	POST_Record[52].UpdateSelf = FALSE;
	POST_Record[52].PValue = &BUF_Text[27];
	pText = POST_Record[52].PValue;
	pText->CurrValue = postbuf_text27[0];
	pText->UserValue = postbuf_text27[1];
	pText->Offset = 2366;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 53 */
	POST_Record[53].Occupy = 1;
	POST_Record[53].PostType = POST_TYPE_TEXT;
	POST_Record[53].Name[0] = 'a';
	POST_Record[53].Name[1] = 'c';
	POST_Record[53].Name[2] = 'c';
	POST_Record[53].Name[3] = '_';
	POST_Record[53].Name[4] = 'i';
	POST_Record[53].Name[5] = 'p';
	POST_Record[53].Name[6] = '2';
	POST_Record[53].NameLen = 7;
	POST_Record[53].FileIndex = 10;
	POST_Record[53].UpdateSelf = FALSE;
	POST_Record[53].PValue = &BUF_Text[28];
	pText = POST_Record[53].PValue;
	pText->CurrValue = postbuf_text28[0];
	pText->UserValue = postbuf_text28[1];
	pText->Offset = 2467;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 54 */
	POST_Record[54].Occupy = 1;
	POST_Record[54].PostType = POST_TYPE_TEXT;
	POST_Record[54].Name[0] = 'a';
	POST_Record[54].Name[1] = 'c';
	POST_Record[54].Name[2] = 'c';
	POST_Record[54].Name[3] = '_';
	POST_Record[54].Name[4] = 'i';
	POST_Record[54].Name[5] = 'p';
	POST_Record[54].Name[6] = '3';
	POST_Record[54].NameLen = 7;
	POST_Record[54].FileIndex = 10;
	POST_Record[54].UpdateSelf = FALSE;
	POST_Record[54].PValue = &BUF_Text[29];
	pText = POST_Record[54].PValue;
	pText->CurrValue = postbuf_text29[0];
	pText->UserValue = postbuf_text29[1];
	pText->Offset = 2568;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 55 */
	POST_Record[55].Occupy = 1;
	POST_Record[55].PostType = POST_TYPE_TEXT;
	POST_Record[55].Name[0] = 'a';
	POST_Record[55].Name[1] = 'c';
	POST_Record[55].Name[2] = 'c';
	POST_Record[55].Name[3] = '_';
	POST_Record[55].Name[4] = 'i';
	POST_Record[55].Name[5] = 'p';
	POST_Record[55].Name[6] = '4';
	POST_Record[55].NameLen = 7;
	POST_Record[55].FileIndex = 10;
	POST_Record[55].UpdateSelf = FALSE;
	POST_Record[55].PValue = &BUF_Text[30];
	pText = POST_Record[55].PValue;
	pText->CurrValue = postbuf_text30[0];
	pText->UserValue = postbuf_text30[1];
	pText->Offset = 2669;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 56 */
	POST_Record[56].Occupy = 1;
	POST_Record[56].PostType = POST_TYPE_SELECT;
	POST_Record[56].Name[0] = 'a';
	POST_Record[56].Name[1] = 'c';
	POST_Record[56].Name[2] = 'c';
	POST_Record[56].Name[3] = '_';
	POST_Record[56].Name[4] = 'i';
	POST_Record[56].Name[5] = 'p';
	POST_Record[56].Name[6] = 'e';
	POST_Record[56].Name[7] = 'n';
	POST_Record[56].NameLen = 8;
	POST_Record[56].FileIndex = 10;
	POST_Record[56].UpdateSelf = FALSE;
	POST_Record[56].PValue = &BUF_Select[13];
	pSelect = POST_Record[56].PValue;
	pSelect->Offset[0] = 2878;
	pSelect->Offset[1] = 2933;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 57 */
	POST_Record[57].Occupy = 1;
	POST_Record[57].PostType = POST_TYPE_SELECT;
	POST_Record[57].Name[0] = 'n';
	POST_Record[57].Name[1] = 'e';
	POST_Record[57].Name[2] = 't';
	POST_Record[57].Name[3] = 'w';
	POST_Record[57].Name[4] = 'o';
	POST_Record[57].Name[5] = 'r';
	POST_Record[57].Name[6] = 'k';
	POST_Record[57].Name[7] = '_';
	POST_Record[57].Name[8] = 'm';
	POST_Record[57].Name[9] = 'o';
	POST_Record[57].Name[10] = 'd';
	POST_Record[57].Name[11] = 'e';
	POST_Record[57].NameLen = 12;
	POST_Record[57].FileIndex = 14;
	POST_Record[57].UpdateSelf = FALSE;
	POST_Record[57].PValue = &BUF_Select[14];
	pSelect = POST_Record[57].PValue;
	pSelect->Offset[0] = 1322;
	pSelect->Offset[1] = 1385;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 58 */
	POST_Record[58].Occupy = 1;
	POST_Record[58].PostType = POST_TYPE_SELECT;
	POST_Record[58].Name[0] = 'c';
	POST_Record[58].Name[1] = 'h';
	POST_Record[58].Name[2] = 'a';
	POST_Record[58].Name[3] = 'n';
	POST_Record[58].Name[4] = 'n';
	POST_Record[58].Name[5] = 'e';
	POST_Record[58].Name[6] = 'l';
	POST_Record[58].NameLen = 7;
	POST_Record[58].FileIndex = 14;
	POST_Record[58].UpdateSelf = FALSE;
	POST_Record[58].PValue = &BUF_Select[15];
	pSelect = POST_Record[58].PValue;
	pSelect->Offset[0] = 1499;
	pSelect->Offset[1] = 1552;
	pSelect->Offset[2] = 1593;
	pSelect->Offset[3] = 1634;
	pSelect->Offset[4] = 1675;
	pSelect->Offset[5] = 1716;
	pSelect->Offset[6] = 1757;
	pSelect->Offset[7] = 1798;
	pSelect->Offset[8] = 1839;
	pSelect->Offset[9] = 1880;
	pSelect->Offset[10] = 1921;
	pSelect->Offset[11] = 1963;
	pSelect->Count = 12;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 59 */
	POST_Record[59].Occupy = 1;
	POST_Record[59].PostType = POST_TYPE_TEXT;
	POST_Record[59].Name[0] = 's';
	POST_Record[59].Name[1] = 's';
	POST_Record[59].Name[2] = 'i';
	POST_Record[59].Name[3] = 'd';
	POST_Record[59].NameLen = 4;
	POST_Record[59].FileIndex = 14;
	POST_Record[59].UpdateSelf = FALSE;
	POST_Record[59].PValue = &BUF_Text[31];
	pText = POST_Record[59].PValue;
	pText->CurrValue = postbuf_text31[0];
	pText->UserValue = postbuf_text31[1];
	pText->Offset = 2082;
	pText->DefaultLength = 9;
	pText->CurrValue[0] = 'a';
	pText->CurrValue[1] = 's';
	pText->CurrValue[2] = 'i';
	pText->CurrValue[3] = 'x';
	pText->CurrValue[4] = 't';
	pText->CurrValue[5] = 'e';
	pText->CurrValue[6] = 's';
	pText->CurrValue[7] = 't';
	pText->CurrValue[8] = '"';
	pText->CurrLength = 9;

	/* for record 60 */
	POST_Record[60].Occupy = 1;
	POST_Record[60].PostType = POST_TYPE_SELECT;
	POST_Record[60].Name[0] = 's';
	POST_Record[60].Name[1] = 'e';
	POST_Record[60].Name[2] = 'c';
	POST_Record[60].Name[3] = 'u';
	POST_Record[60].Name[4] = 'r';
	POST_Record[60].Name[5] = 'i';
	POST_Record[60].Name[6] = 't';
	POST_Record[60].Name[7] = 'y';
	POST_Record[60].Name[8] = '_';
	POST_Record[60].Name[9] = 'm';
	POST_Record[60].Name[10] = 'o';
	POST_Record[60].Name[11] = 'd';
	POST_Record[60].Name[12] = 'e';
	POST_Record[60].NameLen = 13;
	POST_Record[60].FileIndex = 14;
	POST_Record[60].UpdateSelf = FALSE;
	POST_Record[60].PValue = &BUF_Select[16];
	pSelect = POST_Record[60].PValue;
	pSelect->Offset[0] = 2207;
	pSelect->Offset[1] = 2267;
	pSelect->Offset[2] = 2313;
	pSelect->Offset[3] = 2360;
	pSelect->Offset[4] = 2404;
	pSelect->Count = 5;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 61 */
	POST_Record[61].Occupy = 1;
	POST_Record[61].PostType = POST_TYPE_SELECT;
	POST_Record[61].Name[0] = 'k';
	POST_Record[61].Name[1] = 'e';
	POST_Record[61].Name[2] = 'y';
	POST_Record[61].Name[3] = '_';
	POST_Record[61].Name[4] = 'l';
	POST_Record[61].Name[5] = 'e';
	POST_Record[61].Name[6] = 'n';
	POST_Record[61].Name[7] = 'g';
	POST_Record[61].Name[8] = 't';
	POST_Record[61].Name[9] = 'h';
	POST_Record[61].NameLen = 10;
	POST_Record[61].FileIndex = 14;
	POST_Record[61].UpdateSelf = FALSE;
	POST_Record[61].PValue = &BUF_Select[17];
	pSelect = POST_Record[61].PValue;
	pSelect->Offset[0] = 2618;
	pSelect->Offset[1] = 2674;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 62 */
	POST_Record[62].Occupy = 1;
	POST_Record[62].PostType = POST_TYPE_SELECT;
	POST_Record[62].Name[0] = 'k';
	POST_Record[62].Name[1] = 'e';
	POST_Record[62].Name[2] = 'y';
	POST_Record[62].Name[3] = '_';
	POST_Record[62].Name[4] = 'i';
	POST_Record[62].Name[5] = 'n';
	POST_Record[62].Name[6] = 'd';
	POST_Record[62].Name[7] = 'e';
	POST_Record[62].Name[8] = 'x';
	POST_Record[62].NameLen = 9;
	POST_Record[62].FileIndex = 14;
	POST_Record[62].UpdateSelf = FALSE;
	POST_Record[62].PValue = &BUF_Select[18];
	pSelect = POST_Record[62].PValue;
	pSelect->Offset[0] = 2801;
	pSelect->Offset[1] = 2861;
	pSelect->Offset[2] = 2912;
	pSelect->Offset[3] = 2963;
	pSelect->Count = 4;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 63 */
	POST_Record[63].Occupy = 1;
	POST_Record[63].PostType = POST_TYPE_TEXT;
	POST_Record[63].Name[0] = 'k';
	POST_Record[63].Name[1] = 'e';
	POST_Record[63].Name[2] = 'y';
	POST_Record[63].Name[3] = '_';
	POST_Record[63].Name[4] = 'i';
	POST_Record[63].Name[5] = 'n';
	POST_Record[63].Name[6] = 'd';
	POST_Record[63].Name[7] = 'e';
	POST_Record[63].Name[8] = 'x';
	POST_Record[63].Name[9] = '_';
	POST_Record[63].Name[10] = '0';
	POST_Record[63].NameLen = 11;
	POST_Record[63].FileIndex = 14;
	POST_Record[63].UpdateSelf = FALSE;
	POST_Record[63].PValue = &BUF_Text[32];
	pText = POST_Record[63].PValue;
	pText->CurrValue = postbuf_text32[0];
	pText->UserValue = postbuf_text32[1];
	pText->Offset = 3104;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 64 */
	POST_Record[64].Occupy = 1;
	POST_Record[64].PostType = POST_TYPE_TEXT;
	POST_Record[64].Name[0] = 'k';
	POST_Record[64].Name[1] = 'e';
	POST_Record[64].Name[2] = 'y';
	POST_Record[64].Name[3] = '_';
	POST_Record[64].Name[4] = 'i';
	POST_Record[64].Name[5] = 'n';
	POST_Record[64].Name[6] = 'd';
	POST_Record[64].Name[7] = 'e';
	POST_Record[64].Name[8] = 'x';
	POST_Record[64].Name[9] = '_';
	POST_Record[64].Name[10] = '1';
	POST_Record[64].NameLen = 11;
	POST_Record[64].FileIndex = 14;
	POST_Record[64].UpdateSelf = FALSE;
	POST_Record[64].PValue = &BUF_Text[33];
	pText = POST_Record[64].PValue;
	pText->CurrValue = postbuf_text33[0];
	pText->UserValue = postbuf_text33[1];
	pText->Offset = 3251;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 65 */
	POST_Record[65].Occupy = 1;
	POST_Record[65].PostType = POST_TYPE_TEXT;
	POST_Record[65].Name[0] = 'k';
	POST_Record[65].Name[1] = 'e';
	POST_Record[65].Name[2] = 'y';
	POST_Record[65].Name[3] = '_';
	POST_Record[65].Name[4] = 'i';
	POST_Record[65].Name[5] = 'n';
	POST_Record[65].Name[6] = 'd';
	POST_Record[65].Name[7] = 'e';
	POST_Record[65].Name[8] = 'x';
	POST_Record[65].Name[9] = '_';
	POST_Record[65].Name[10] = '2';
	POST_Record[65].NameLen = 11;
	POST_Record[65].FileIndex = 14;
	POST_Record[65].UpdateSelf = FALSE;
	POST_Record[65].PValue = &BUF_Text[34];
	pText = POST_Record[65].PValue;
	pText->CurrValue = postbuf_text34[0];
	pText->UserValue = postbuf_text34[1];
	pText->Offset = 3399;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 66 */
	POST_Record[66].Occupy = 1;
	POST_Record[66].PostType = POST_TYPE_TEXT;
	POST_Record[66].Name[0] = 'k';
	POST_Record[66].Name[1] = 'e';
	POST_Record[66].Name[2] = 'y';
	POST_Record[66].Name[3] = '_';
	POST_Record[66].Name[4] = 'i';
	POST_Record[66].Name[5] = 'n';
	POST_Record[66].Name[6] = 'd';
	POST_Record[66].Name[7] = 'e';
	POST_Record[66].Name[8] = 'x';
	POST_Record[66].Name[9] = '_';
	POST_Record[66].Name[10] = '3';
	POST_Record[66].NameLen = 11;
	POST_Record[66].FileIndex = 14;
	POST_Record[66].UpdateSelf = FALSE;
	POST_Record[66].PValue = &BUF_Text[35];
	pText = POST_Record[66].PValue;
	pText->CurrValue = postbuf_text35[0];
	pText->UserValue = postbuf_text35[1];
	pText->Offset = 3547;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 67 */
	POST_Record[67].Occupy = 1;
	POST_Record[67].PostType = POST_TYPE_TEXT;
	POST_Record[67].Name[0] = 'a';
	POST_Record[67].Name[1] = 'e';
	POST_Record[67].Name[2] = 's';
	POST_Record[67].Name[3] = '_';
	POST_Record[67].Name[4] = 'p';
	POST_Record[67].Name[5] = 'a';
	POST_Record[67].Name[6] = 's';
	POST_Record[67].Name[7] = 's';
	POST_Record[67].Name[8] = 'p';
	POST_Record[67].Name[9] = 'h';
	POST_Record[67].Name[10] = 'r';
	POST_Record[67].Name[11] = 'a';
	POST_Record[67].Name[12] = 's';
	POST_Record[67].Name[13] = 'e';
	POST_Record[67].NameLen = 14;
	POST_Record[67].FileIndex = 14;
	POST_Record[67].UpdateSelf = FALSE;
	POST_Record[67].PValue = &BUF_Text[36];
	pText = POST_Record[67].PValue;
	pText->CurrValue = postbuf_text36[0];
	pText->UserValue = postbuf_text36[1];
	pText->Offset = 3893;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 68 */
	POST_Record[68].Occupy = 1;
	POST_Record[68].PostType = POST_TYPE_SELECT;
	POST_Record[68].Name[0] = 't';
	POST_Record[68].Name[1] = 'x';
	POST_Record[68].Name[2] = '_';
	POST_Record[68].Name[3] = 'd';
	POST_Record[68].Name[4] = 'a';
	POST_Record[68].Name[5] = 't';
	POST_Record[68].Name[6] = 'a';
	POST_Record[68].Name[7] = '_';
	POST_Record[68].Name[8] = 'r';
	POST_Record[68].Name[9] = 'a';
	POST_Record[68].Name[10] = 't';
	POST_Record[68].Name[11] = 'e';
	POST_Record[68].NameLen = 12;
	POST_Record[68].FileIndex = 14;
	POST_Record[68].UpdateSelf = FALSE;
	POST_Record[68].PValue = &BUF_Select[19];
	pSelect = POST_Record[68].PValue;
	pSelect->Offset[0] = 4414;
	pSelect->Offset[1] = 4472;
	pSelect->Offset[2] = 4523;
	pSelect->Offset[3] = 4574;
	pSelect->Offset[4] = 4627;
	pSelect->Offset[5] = 4678;
	pSelect->Offset[6] = 4729;
	pSelect->Offset[7] = 4781;
	pSelect->Offset[8] = 4833;
	pSelect->Offset[9] = 4885;
	pSelect->Offset[10] = 4937;
	pSelect->Offset[11] = 4989;
	pSelect->Offset[12] = 5041;
	pSelect->Count = 13;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 69 */
	POST_Record[69].Occupy = 1;
	POST_Record[69].PostType = POST_TYPE_SELECT;
	POST_Record[69].Name[0] = 't';
	POST_Record[69].Name[1] = 'x';
	POST_Record[69].Name[2] = '_';
	POST_Record[69].Name[3] = 'p';
	POST_Record[69].Name[4] = 'o';
	POST_Record[69].Name[5] = 'w';
	POST_Record[69].Name[6] = 'e';
	POST_Record[69].Name[7] = 'r';
	POST_Record[69].NameLen = 8;
	POST_Record[69].FileIndex = 14;
	POST_Record[69].UpdateSelf = FALSE;
	POST_Record[69].PValue = &BUF_Select[20];
	pSelect = POST_Record[69].PValue;
	pSelect->Offset[0] = 5179;
	pSelect->Offset[1] = 5237;
	pSelect->Offset[2] = 5285;
	pSelect->Offset[3] = 5333;
	pSelect->Offset[4] = 5383;
	pSelect->Count = 5;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 70 */
	POST_Record[70].Occupy = 1;
	POST_Record[70].PostType = POST_TYPE_SELECT;
	POST_Record[70].Name[0] = 'p';
	POST_Record[70].Name[1] = 'r';
	POST_Record[70].Name[2] = 'e';
	POST_Record[70].Name[3] = 'a';
	POST_Record[70].Name[4] = 'm';
	POST_Record[70].Name[5] = 'b';
	POST_Record[70].Name[6] = 'l';
	POST_Record[70].Name[7] = 'e';
	POST_Record[70].NameLen = 8;
	POST_Record[70].FileIndex = 14;
	POST_Record[70].UpdateSelf = FALSE;
	POST_Record[70].PValue = &BUF_Select[21];
	pSelect = POST_Record[70].PValue;
	pSelect->Offset[0] = 5508;
	pSelect->Offset[1] = 5566;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 71 */
	POST_Record[71].Occupy = 1;
	POST_Record[71].PostType = POST_TYPE_TEXT;
	POST_Record[71].Name[0] = 'b';
	POST_Record[71].Name[1] = 'e';
	POST_Record[71].Name[2] = 'a';
	POST_Record[71].Name[3] = 'c';
	POST_Record[71].Name[4] = 'o';
	POST_Record[71].Name[5] = 'n';
	POST_Record[71].Name[6] = '_';
	POST_Record[71].Name[7] = 'i';
	POST_Record[71].Name[8] = 'n';
	POST_Record[71].Name[9] = 't';
	POST_Record[71].Name[10] = 'v';
	POST_Record[71].NameLen = 11;
	POST_Record[71].FileIndex = 14;
	POST_Record[71].UpdateSelf = FALSE;
	POST_Record[71].PValue = &BUF_Text[37];
	pText = POST_Record[71].PValue;
	pText->CurrValue = postbuf_text37[0];
	pText->UserValue = postbuf_text37[1];
	pText->Offset = 5692;
	pText->DefaultLength = 4;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '0';
	pText->CurrValue[2] = '0';
	pText->CurrValue[3] = '"';
	pText->CurrLength = 4;

	/* for record 72 */
	POST_Record[72].Occupy = 1;
	POST_Record[72].PostType = POST_TYPE_TEXT;
	POST_Record[72].Name[0] = 'r';
	POST_Record[72].Name[1] = 't';
	POST_Record[72].Name[2] = 's';
	POST_Record[72].Name[3] = '_';
	POST_Record[72].Name[4] = 't';
	POST_Record[72].Name[5] = 'h';
	POST_Record[72].Name[6] = 'r';
	POST_Record[72].Name[7] = 'e';
	POST_Record[72].Name[8] = 's';
	POST_Record[72].Name[9] = 'h';
	POST_Record[72].Name[10] = 'o';
	POST_Record[72].Name[11] = 'l';
	POST_Record[72].Name[12] = 'd';
	POST_Record[72].NameLen = 13;
	POST_Record[72].FileIndex = 14;
	POST_Record[72].UpdateSelf = FALSE;
	POST_Record[72].PValue = &BUF_Text[38];
	pText = POST_Record[72].PValue;
	pText->CurrValue = postbuf_text38[0];
	pText->UserValue = postbuf_text38[1];
	pText->Offset = 5903;
	pText->DefaultLength = 5;
	pText->CurrValue[0] = '2';
	pText->CurrValue[1] = '4';
	pText->CurrValue[2] = '3';
	pText->CurrValue[3] = '2';
	pText->CurrValue[4] = '"';
	pText->CurrLength = 5;

	/* for record 73 */
	POST_Record[73].Occupy = 1;
	POST_Record[73].PostType = POST_TYPE_SELECT;
	POST_Record[73].Name[0] = 'a';
	POST_Record[73].Name[1] = 'u';
	POST_Record[73].Name[2] = 't';
	POST_Record[73].Name[3] = 'o';
	POST_Record[73].Name[4] = '_';
	POST_Record[73].Name[5] = 'p';
	POST_Record[73].Name[6] = 'w';
	POST_Record[73].Name[7] = 'r';
	POST_Record[73].Name[8] = '_';
	POST_Record[73].Name[9] = 'c';
	POST_Record[73].Name[10] = 't';
	POST_Record[73].Name[11] = 'r';
	POST_Record[73].Name[12] = 'l';
	POST_Record[73].NameLen = 13;
	POST_Record[73].FileIndex = 14;
	POST_Record[73].UpdateSelf = FALSE;
	POST_Record[73].PValue = &BUF_Select[22];
	pSelect = POST_Record[73].PValue;
	pSelect->Offset[0] = 6132;
	pSelect->Offset[1] = 6193;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 74 */
	POST_Record[74].Occupy = 1;
	POST_Record[74].PostType = POST_TYPE_SELECT;
	POST_Record[74].Name[0] = 'p';
	POST_Record[74].Name[1] = 'r';
	POST_Record[74].Name[2] = 'o';
	POST_Record[74].Name[3] = 't';
	POST_Record[74].Name[4] = 'f';
	POST_Record[74].Name[5] = 'r';
	POST_Record[74].Name[6] = 'm';
	POST_Record[74].Name[7] = '_';
	POST_Record[74].Name[8] = 't';
	POST_Record[74].Name[9] = 'y';
	POST_Record[74].Name[10] = 'p';
	POST_Record[74].Name[11] = 'e';
	POST_Record[74].NameLen = 12;
	POST_Record[74].FileIndex = 14;
	POST_Record[74].UpdateSelf = FALSE;
	POST_Record[74].PValue = &BUF_Select[23];
	pSelect = POST_Record[74].PValue;
	pSelect->Offset[0] = 6331;
	pSelect->Offset[1] = 6388;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 75 */
	POST_Record[75].Occupy = 1;
	POST_Record[75].PostType = POST_TYPE_SELECT;
	POST_Record[75].Name[0] = 'w';
	POST_Record[75].Name[1] = 'z';
	POST_Record[75].Name[2] = 't';
	POST_Record[75].Name[3] = 'x';
	POST_Record[75].Name[4] = 'd';
	POST_Record[75].Name[5] = 'a';
	POST_Record[75].Name[6] = 't';
	POST_Record[75].Name[7] = 'a';
	POST_Record[75].Name[8] = 'r';
	POST_Record[75].Name[9] = 'a';
	POST_Record[75].Name[10] = 't';
	POST_Record[75].Name[11] = 'e';
	POST_Record[75].NameLen = 12;
	POST_Record[75].FileIndex = 15;
	POST_Record[75].UpdateSelf = FALSE;
	POST_Record[75].PValue = &BUF_Select[24];
	pSelect = POST_Record[75].PValue;
	pSelect->Offset[0] = 1360;
	pSelect->Offset[1] = 1416;
	pSelect->Offset[2] = 1465;
	pSelect->Offset[3] = 1514;
	pSelect->Offset[4] = 1565;
	pSelect->Offset[5] = 1614;
	pSelect->Offset[6] = 1663;
	pSelect->Offset[7] = 1713;
	pSelect->Offset[8] = 1763;
	pSelect->Offset[9] = 1813;
	pSelect->Offset[10] = 1863;
	pSelect->Offset[11] = 1913;
	pSelect->Offset[12] = 1963;
	pSelect->Count = 13;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 76 */
	POST_Record[76].Occupy = 1;
	POST_Record[76].PostType = POST_TYPE_SELECT;
	POST_Record[76].Name[0] = 'w';
	POST_Record[76].Name[1] = 'z';
	POST_Record[76].Name[2] = 't';
	POST_Record[76].Name[3] = 'x';
	POST_Record[76].Name[4] = 'p';
	POST_Record[76].Name[5] = 'o';
	POST_Record[76].Name[6] = 'w';
	POST_Record[76].Name[7] = 'e';
	POST_Record[76].Name[8] = 'r';
	POST_Record[76].NameLen = 9;
	POST_Record[76].FileIndex = 15;
	POST_Record[76].UpdateSelf = FALSE;
	POST_Record[76].PValue = &BUF_Select[25];
	pSelect = POST_Record[76].PValue;
	pSelect->Offset[0] = 2166;
	pSelect->Offset[1] = 2222;
	pSelect->Offset[2] = 2268;
	pSelect->Offset[3] = 2314;
	pSelect->Offset[4] = 2362;
	pSelect->Count = 5;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 77 */
	POST_Record[77].Occupy = 1;
	POST_Record[77].PostType = POST_TYPE_SELECT;
	POST_Record[77].Name[0] = 'w';
	POST_Record[77].Name[1] = 'z';
	POST_Record[77].Name[2] = 'p';
	POST_Record[77].Name[3] = 'r';
	POST_Record[77].Name[4] = 'e';
	POST_Record[77].Name[5] = 'a';
	POST_Record[77].Name[6] = 'm';
	POST_Record[77].Name[7] = 'b';
	POST_Record[77].Name[8] = 'l';
	POST_Record[77].Name[9] = 'e';
	POST_Record[77].NameLen = 10;
	POST_Record[77].FileIndex = 15;
	POST_Record[77].UpdateSelf = FALSE;
	POST_Record[77].PValue = &BUF_Select[26];
	pSelect = POST_Record[77].PValue;
	pSelect->Offset[0] = 2553;
	pSelect->Offset[1] = 2609;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 78 */
	POST_Record[78].Occupy = 1;
	POST_Record[78].PostType = POST_TYPE_TEXT;
	POST_Record[78].Name[0] = 'w';
	POST_Record[78].Name[1] = 'z';
	POST_Record[78].Name[2] = 'b';
	POST_Record[78].Name[3] = 'e';
	POST_Record[78].Name[4] = 'a';
	POST_Record[78].Name[5] = 'c';
	POST_Record[78].Name[6] = 'o';
	POST_Record[78].Name[7] = 'n';
	POST_Record[78].Name[8] = 'i';
	POST_Record[78].Name[9] = 'n';
	POST_Record[78].Name[10] = 't';
	POST_Record[78].Name[11] = 'v';
	POST_Record[78].NameLen = 12;
	POST_Record[78].FileIndex = 15;
	POST_Record[78].UpdateSelf = FALSE;
	POST_Record[78].PValue = &BUF_Text[39];
	pText = POST_Record[78].PValue;
	pText->CurrValue = postbuf_text39[0];
	pText->UserValue = postbuf_text39[1];
	pText->Offset = 2955;
	pText->DefaultLength = 4;
	pText->CurrValue[0] = '1';
	pText->CurrValue[1] = '0';
	pText->CurrValue[2] = '0';
	pText->CurrValue[3] = '"';
	pText->CurrLength = 4;

	/* for record 79 */
	POST_Record[79].Occupy = 1;
	POST_Record[79].PostType = POST_TYPE_TEXT;
	POST_Record[79].Name[0] = 'w';
	POST_Record[79].Name[1] = 'z';
	POST_Record[79].Name[2] = 'r';
	POST_Record[79].Name[3] = 't';
	POST_Record[79].Name[4] = 's';
	POST_Record[79].Name[5] = 't';
	POST_Record[79].Name[6] = 'h';
	POST_Record[79].Name[7] = 'r';
	POST_Record[79].Name[8] = 'e';
	POST_Record[79].Name[9] = 's';
	POST_Record[79].Name[10] = 'h';
	POST_Record[79].Name[11] = 'o';
	POST_Record[79].Name[12] = 'l';
	POST_Record[79].Name[13] = 'd';
	POST_Record[79].NameLen = 14;
	POST_Record[79].FileIndex = 15;
	POST_Record[79].UpdateSelf = FALSE;
	POST_Record[79].PValue = &BUF_Text[40];
	pText = POST_Record[79].PValue;
	pText->CurrValue = postbuf_text40[0];
	pText->UserValue = postbuf_text40[1];
	pText->Offset = 3283;
	pText->DefaultLength = 5;
	pText->CurrValue[0] = '2';
	pText->CurrValue[1] = '4';
	pText->CurrValue[2] = '3';
	pText->CurrValue[3] = '2';
	pText->CurrValue[4] = '"';
	pText->CurrLength = 5;

	/* for record 80 */
	POST_Record[80].Occupy = 1;
	POST_Record[80].PostType = POST_TYPE_SELECT;
	POST_Record[80].Name[0] = 'w';
	POST_Record[80].Name[1] = 'z';
	POST_Record[80].Name[2] = 'a';
	POST_Record[80].Name[3] = 'u';
	POST_Record[80].Name[4] = 't';
	POST_Record[80].Name[5] = 'o';
	POST_Record[80].Name[6] = 'p';
	POST_Record[80].Name[7] = 'w';
	POST_Record[80].Name[8] = 'r';
	POST_Record[80].Name[9] = 'c';
	POST_Record[80].Name[10] = 't';
	POST_Record[80].Name[11] = 'r';
	POST_Record[80].Name[12] = 'l';
	POST_Record[80].NameLen = 13;
	POST_Record[80].FileIndex = 15;
	POST_Record[80].UpdateSelf = FALSE;
	POST_Record[80].PValue = &BUF_Select[27];
	pSelect = POST_Record[80].PValue;
	pSelect->Offset[0] = 3479;
	pSelect->Offset[1] = 3538;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 81 */
	POST_Record[81].Occupy = 1;
	POST_Record[81].PostType = POST_TYPE_SELECT;
	POST_Record[81].Name[0] = 'w';
	POST_Record[81].Name[1] = 'z';
	POST_Record[81].Name[2] = 'p';
	POST_Record[81].Name[3] = 'r';
	POST_Record[81].Name[4] = 'o';
	POST_Record[81].Name[5] = 't';
	POST_Record[81].Name[6] = 'f';
	POST_Record[81].Name[7] = 'r';
	POST_Record[81].Name[8] = 'm';
	POST_Record[81].Name[9] = '_';
	POST_Record[81].Name[10] = 't';
	POST_Record[81].Name[11] = 'y';
	POST_Record[81].Name[12] = 'p';
	POST_Record[81].Name[13] = 'e';
	POST_Record[81].NameLen = 14;
	POST_Record[81].FileIndex = 15;
	POST_Record[81].UpdateSelf = FALSE;
	POST_Record[81].PValue = &BUF_Select[28];
	pSelect = POST_Record[81].PValue;
	pSelect->Offset[0] = 3742;
	pSelect->Offset[1] = 3797;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 82 */
	POST_Record[82].Occupy = 1;
	POST_Record[82].PostType = POST_TYPE_SUBMIT;
	POST_Record[82].Name[0] = 'N';
	POST_Record[82].Name[1] = 'e';
	POST_Record[82].Name[2] = 'x';
	POST_Record[82].Name[3] = 't';
	POST_Record[82].NameLen = 4;
	POST_Record[82].FileIndex = 0xff;
	POST_Record[82].UpdateSelf = FALSE;
	POST_Record[82].PValue = &BUF_Submit[7];
	pSubmit = POST_Record[82].PValue;
	pSubmit->Value[0] = 'N';
	pSubmit->Value[1] = 'e';
	pSubmit->Value[2] = 'x';
	pSubmit->Value[3] = 't';
	pSubmit->DefaultVlaueLen = 4;
	pSubmit->IsApply = 1;

	/* for record 83 */
	POST_Record[83].Occupy = 1;
	POST_Record[83].PostType = POST_TYPE_SUBMIT;
	POST_Record[83].Name[0] = 'B';
	POST_Record[83].Name[1] = 'a';
	POST_Record[83].Name[2] = 'c';
	POST_Record[83].Name[3] = 'k';
	POST_Record[83].NameLen = 4;
	POST_Record[83].FileIndex = 0xff;
	POST_Record[83].UpdateSelf = FALSE;
	POST_Record[83].PValue = &BUF_Submit[8];
	pSubmit = POST_Record[83].PValue;
	pSubmit->Value[0] = 'B';
	pSubmit->Value[1] = 'a';
	pSubmit->Value[2] = 'c';
	pSubmit->Value[3] = 'k';
	pSubmit->DefaultVlaueLen = 4;
	pSubmit->IsApply = 1;

	/* for record 84 */
	POST_Record[84].Occupy = 1;
	POST_Record[84].PostType = POST_TYPE_RADIO;
	POST_Record[84].Name[0] = 'w';
	POST_Record[84].Name[1] = 'e';
	POST_Record[84].Name[2] = 'p';
	POST_Record[84].NameLen = 3;
	POST_Record[84].FileIndex = 16;
	POST_Record[84].UpdateSelf = FALSE;
	POST_Record[84].PValue = &BUF_Radio[1];
	pRadio = POST_Record[84].PValue;
	pRadio->Offset[0] = 1357;
	pRadio->Value[0][0] = '0';
	pRadio->Length[0] = 1;
	pRadio->Offset[1] = 1501;
	pRadio->Value[1][0] = '1';
	pRadio->Length[1] = 1;
	pRadio->Offset[2] = 1650;
	pRadio->Value[2][0] = '2';
	pRadio->Length[2] = 1;
	pRadio->Count = 3;
	pRadio->DefaultSet = 2;
	pRadio->CurrentSet = 2;
	pRadio->UserSet = 2;

	/* for record 85 */
	POST_Record[85].Occupy = 1;
	POST_Record[85].PostType = POST_TYPE_SELECT;
	POST_Record[85].Name[0] = 'w';
	POST_Record[85].Name[1] = 'z';
	POST_Record[85].Name[2] = 'n';
	POST_Record[85].Name[3] = 'e';
	POST_Record[85].Name[4] = 't';
	POST_Record[85].Name[5] = 'w';
	POST_Record[85].Name[6] = 'o';
	POST_Record[85].Name[7] = 'r';
	POST_Record[85].Name[8] = 'k';
	POST_Record[85].Name[9] = 'm';
	POST_Record[85].Name[10] = 'o';
	POST_Record[85].Name[11] = 'd';
	POST_Record[85].Name[12] = 'e';
	POST_Record[85].NameLen = 13;
	POST_Record[85].FileIndex = 19;
	POST_Record[85].UpdateSelf = FALSE;
	POST_Record[85].PValue = &BUF_Select[29];
	pSelect = POST_Record[85].PValue;
	pSelect->Offset[0] = 1517;
	pSelect->Offset[1] = 1585;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 86 */
	POST_Record[86].Occupy = 1;
	POST_Record[86].PostType = POST_TYPE_SELECT;
	POST_Record[86].Name[0] = 'w';
	POST_Record[86].Name[1] = 'z';
	POST_Record[86].Name[2] = 'c';
	POST_Record[86].Name[3] = 'h';
	POST_Record[86].Name[4] = 'a';
	POST_Record[86].Name[5] = 'n';
	POST_Record[86].Name[6] = 'n';
	POST_Record[86].Name[7] = 'e';
	POST_Record[86].Name[8] = 'l';
	POST_Record[86].NameLen = 9;
	POST_Record[86].FileIndex = 19;
	POST_Record[86].UpdateSelf = FALSE;
	POST_Record[86].PValue = &BUF_Select[30];
	pSelect = POST_Record[86].PValue;
	pSelect->Offset[0] = 1772;
	pSelect->Offset[1] = 1830;
	pSelect->Offset[2] = 1876;
	pSelect->Offset[3] = 1922;
	pSelect->Offset[4] = 1968;
	pSelect->Offset[5] = 2014;
	pSelect->Offset[6] = 2060;
	pSelect->Offset[7] = 2106;
	pSelect->Offset[8] = 2152;
	pSelect->Offset[9] = 2198;
	pSelect->Offset[10] = 2244;
	pSelect->Offset[11] = 2291;
	pSelect->Count = 12;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 87 */
	POST_Record[87].Occupy = 1;
	POST_Record[87].PostType = POST_TYPE_TEXT;
	POST_Record[87].Name[0] = 'w';
	POST_Record[87].Name[1] = 'z';
	POST_Record[87].Name[2] = 's';
	POST_Record[87].Name[3] = 's';
	POST_Record[87].Name[4] = 'i';
	POST_Record[87].Name[5] = 'd';
	POST_Record[87].NameLen = 6;
	POST_Record[87].FileIndex = 19;
	POST_Record[87].UpdateSelf = FALSE;
	POST_Record[87].PValue = &BUF_Text[41];
	pText = POST_Record[87].PValue;
	pText->CurrValue = postbuf_text41[0];
	pText->UserValue = postbuf_text41[1];
	pText->Offset = 2481;
	pText->DefaultLength = 9;
	pText->CurrValue[0] = 'a';
	pText->CurrValue[1] = 's';
	pText->CurrValue[2] = 'i';
	pText->CurrValue[3] = 'x';
	pText->CurrValue[4] = 't';
	pText->CurrValue[5] = 'e';
	pText->CurrValue[6] = 's';
	pText->CurrValue[7] = 't';
	pText->CurrValue[8] = '"';
	pText->CurrLength = 9;

	/* for record 88 */
	POST_Record[88].Occupy = 1;
	POST_Record[88].PostType = POST_TYPE_SELECT;
	POST_Record[88].Name[0] = 'w';
	POST_Record[88].Name[1] = 'z';
	POST_Record[88].Name[2] = 's';
	POST_Record[88].Name[3] = 'c';
	POST_Record[88].Name[4] = 'r';
	POST_Record[88].Name[5] = 't';
	POST_Record[88].Name[6] = 'm';
	POST_Record[88].Name[7] = 'o';
	POST_Record[88].Name[8] = 'd';
	POST_Record[88].Name[9] = 'e';
	POST_Record[88].NameLen = 10;
	POST_Record[88].FileIndex = 19;
	POST_Record[88].UpdateSelf = FALSE;
	POST_Record[88].PValue = &BUF_Select[31];
	pSelect = POST_Record[88].PValue;
	pSelect->Offset[0] = 2781;
	pSelect->Offset[1] = 2846;
	pSelect->Offset[2] = 2897;
	pSelect->Offset[3] = 2949;
	pSelect->Offset[4] = 2998;
	pSelect->Count = 5;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 89 */
	POST_Record[89].Occupy = 1;
	POST_Record[89].PostType = POST_TYPE_SUBMIT;
	POST_Record[89].Name[0] = 'F';
	POST_Record[89].Name[1] = 'i';
	POST_Record[89].Name[2] = 'n';
	POST_Record[89].Name[3] = 'i';
	POST_Record[89].Name[4] = 's';
	POST_Record[89].Name[5] = 'h';
	POST_Record[89].NameLen = 6;
	POST_Record[89].FileIndex = 0xff;
	POST_Record[89].UpdateSelf = FALSE;
	POST_Record[89].PValue = &BUF_Submit[9];
	pSubmit = POST_Record[89].PValue;
	pSubmit->Value[0] = 'F';
	pSubmit->Value[1] = 'i';
	pSubmit->Value[2] = 'n';
	pSubmit->Value[3] = 'i';
	pSubmit->Value[4] = 's';
	pSubmit->Value[5] = 'h';
	pSubmit->DefaultVlaueLen = 6;
	pSubmit->IsApply = 1;

	/* for record 90 */
	POST_Record[90].Occupy = 1;
	POST_Record[90].PostType = POST_TYPE_SELECT;
	POST_Record[90].Name[0] = 'w';
	POST_Record[90].Name[1] = 'z';
	POST_Record[90].Name[2] = 'k';
	POST_Record[90].Name[3] = 'e';
	POST_Record[90].Name[4] = 'y';
	POST_Record[90].Name[5] = 'l';
	POST_Record[90].Name[6] = 'e';
	POST_Record[90].Name[7] = 'n';
	POST_Record[90].Name[8] = 'g';
	POST_Record[90].Name[9] = 't';
	POST_Record[90].Name[10] = 'h';
	POST_Record[90].NameLen = 11;
	POST_Record[90].FileIndex = 20;
	POST_Record[90].UpdateSelf = FALSE;
	POST_Record[90].PValue = &BUF_Select[32];
	pSelect = POST_Record[90].PValue;
	pSelect->Offset[0] = 1427;
	pSelect->Offset[1] = 1486;
	pSelect->Count = 2;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 91 */
	POST_Record[91].Occupy = 1;
	POST_Record[91].PostType = POST_TYPE_SELECT;
	POST_Record[91].Name[0] = 'w';
	POST_Record[91].Name[1] = 'z';
	POST_Record[91].Name[2] = 'k';
	POST_Record[91].Name[3] = 'e';
	POST_Record[91].Name[4] = 'y';
	POST_Record[91].Name[5] = 'i';
	POST_Record[91].Name[6] = 'n';
	POST_Record[91].Name[7] = 'd';
	POST_Record[91].Name[8] = 'e';
	POST_Record[91].Name[9] = 'x';
	POST_Record[91].NameLen = 10;
	POST_Record[91].FileIndex = 20;
	POST_Record[91].UpdateSelf = FALSE;
	POST_Record[91].PValue = &BUF_Select[33];
	pSelect = POST_Record[91].PValue;
	pSelect->Offset[0] = 1676;
	pSelect->Offset[1] = 1739;
	pSelect->Offset[2] = 1793;
	pSelect->Offset[3] = 1847;
	pSelect->Count = 4;
	pSelect->DefaultSet = 0;
	pSelect->CurrentSet = 0;
	pSelect->UserSet = 0;

	/* for record 92 */
	POST_Record[92].Occupy = 1;
	POST_Record[92].PostType = POST_TYPE_TEXT;
	POST_Record[92].Name[0] = 'w';
	POST_Record[92].Name[1] = 'z';
	POST_Record[92].Name[2] = 'k';
	POST_Record[92].Name[3] = 'e';
	POST_Record[92].Name[4] = 'y';
	POST_Record[92].Name[5] = 'i';
	POST_Record[92].Name[6] = 'n';
	POST_Record[92].Name[7] = 'd';
	POST_Record[92].Name[8] = 'e';
	POST_Record[92].Name[9] = 'x';
	POST_Record[92].Name[10] = '0';
	POST_Record[92].NameLen = 11;
	POST_Record[92].FileIndex = 20;
	POST_Record[92].UpdateSelf = FALSE;
	POST_Record[92].PValue = &BUF_Text[42];
	pText = POST_Record[92].PValue;
	pText->CurrValue = postbuf_text42[0];
	pText->UserValue = postbuf_text42[1];
	pText->Offset = 2196;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 93 */
	POST_Record[93].Occupy = 1;
	POST_Record[93].PostType = POST_TYPE_TEXT;
	POST_Record[93].Name[0] = 'w';
	POST_Record[93].Name[1] = 'z';
	POST_Record[93].Name[2] = 'k';
	POST_Record[93].Name[3] = 'e';
	POST_Record[93].Name[4] = 'y';
	POST_Record[93].Name[5] = 'i';
	POST_Record[93].Name[6] = 'n';
	POST_Record[93].Name[7] = 'd';
	POST_Record[93].Name[8] = 'e';
	POST_Record[93].Name[9] = 'x';
	POST_Record[93].Name[10] = '1';
	POST_Record[93].NameLen = 11;
	POST_Record[93].FileIndex = 20;
	POST_Record[93].UpdateSelf = FALSE;
	POST_Record[93].PValue = &BUF_Text[43];
	pText = POST_Record[93].PValue;
	pText->CurrValue = postbuf_text43[0];
	pText->UserValue = postbuf_text43[1];
	pText->Offset = 2386;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 94 */
	POST_Record[94].Occupy = 1;
	POST_Record[94].PostType = POST_TYPE_TEXT;
	POST_Record[94].Name[0] = 'w';
	POST_Record[94].Name[1] = 'z';
	POST_Record[94].Name[2] = 'k';
	POST_Record[94].Name[3] = 'e';
	POST_Record[94].Name[4] = 'y';
	POST_Record[94].Name[5] = 'i';
	POST_Record[94].Name[6] = 'n';
	POST_Record[94].Name[7] = 'd';
	POST_Record[94].Name[8] = 'e';
	POST_Record[94].Name[9] = 'x';
	POST_Record[94].Name[10] = '2';
	POST_Record[94].NameLen = 11;
	POST_Record[94].FileIndex = 20;
	POST_Record[94].UpdateSelf = FALSE;
	POST_Record[94].PValue = &BUF_Text[44];
	pText = POST_Record[94].PValue;
	pText->CurrValue = postbuf_text44[0];
	pText->UserValue = postbuf_text44[1];
	pText->Offset = 2576;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 95 */
	POST_Record[95].Occupy = 1;
	POST_Record[95].PostType = POST_TYPE_TEXT;
	POST_Record[95].Name[0] = 'w';
	POST_Record[95].Name[1] = 'z';
	POST_Record[95].Name[2] = 'k';
	POST_Record[95].Name[3] = 'e';
	POST_Record[95].Name[4] = 'y';
	POST_Record[95].Name[5] = 'i';
	POST_Record[95].Name[6] = 'n';
	POST_Record[95].Name[7] = 'd';
	POST_Record[95].Name[8] = 'e';
	POST_Record[95].Name[9] = 'x';
	POST_Record[95].Name[10] = '3';
	POST_Record[95].NameLen = 11;
	POST_Record[95].FileIndex = 20;
	POST_Record[95].UpdateSelf = FALSE;
	POST_Record[95].PValue = &BUF_Text[45];
	pText = POST_Record[95].PValue;
	pText->CurrValue = postbuf_text45[0];
	pText->UserValue = postbuf_text45[1];
	pText->Offset = 2766;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

	/* for record 96 */
	POST_Record[96].Occupy = 1;
	POST_Record[96].PostType = POST_TYPE_TEXT;
	POST_Record[96].Name[0] = 'w';
	POST_Record[96].Name[1] = 'z';
	POST_Record[96].Name[2] = 'a';
	POST_Record[96].Name[3] = 'e';
	POST_Record[96].Name[4] = 's';
	POST_Record[96].NameLen = 5;
	POST_Record[96].FileIndex = 21;
	POST_Record[96].UpdateSelf = FALSE;
	POST_Record[96].PValue = &BUF_Text[46];
	pText = POST_Record[96].PValue;
	pText->CurrValue = postbuf_text46[0];
	pText->UserValue = postbuf_text46[1];
	pText->Offset = 1373;
	pText->DefaultLength = 1;
	pText->CurrValue[0] = '"';
	pText->CurrLength = 1;

}

/*
 * ----------------------------------------------------------------------------
 * Function Name: FSYS_FindFile
 * Purpose: 
 * Params:
 * Returns:
 * Note:
 * ----------------------------------------------------------------------------
 */
U8_T FSYS_FindFile(U8_T XDATA* pFName)
{
	U8_T		index, find;
	U8_T *pSour;
	U8_T *pDesc;

	for (index = 0; index < MAX_STORE_FILE_NUM; index++)
	{
		if (!FSYS_Manage[index].Occupy)
			continue;

		pSour = pFName;
		pDesc = FSYS_Manage[index].FName;
		find = 1;
		while (*pDesc != 0)
		{
			if (*pSour != *pDesc)
			{
				find = 0;
				break;
			}
			pSour++;
			pDesc++;
		}
		if (find == 1)
			return index;
	}
	return index;
}


/* End of filesys.c */