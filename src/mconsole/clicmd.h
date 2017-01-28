
#ifndef __CLICMD_H__
#define __CLICMD_H__

/* INCLUDE FILE DECLARATIONS */
#include "types.h"

/* NAMING CONSTANT DECLARATIONS */
#define MAX_USER_ACCOUNT 1

/* TYPE DECLARATIONS */

/* GLOBAL VARIABLES */
extern CONSOLE_CmdEntry const FAR CLICMD_userCmdTable[];
extern CONSOLE_Account CLICMD_userTable[];

/* EXPORTED SUBPROGRAM SPECIFICATIONS */ 
U16_T CLICMD_GetCmdTableSize(void);

#endif /* __CLICMD_H__ */

/* End of clicmd.h */