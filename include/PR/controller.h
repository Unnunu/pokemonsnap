#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

/**************************************************************************
 *									  *
 *		 Copyright (C) 1995, Silicon Graphics, Inc.		  *
 *									  *
 *  These coded instructions, statements, and computer programs  contain  *
 *  unpublished  proprietary  information of Silicon Graphics, Inc., and  *
 *  are protected by Federal copyright law.  They  may  not be disclosed  *
 *  to  third  parties  or copied or duplicated in any form, in whole or  *
 *  in part, without the prior written consent of Silicon Graphics, Inc.  *
 *									  *
 **************************************************************************/

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo. (Originated by SGI)
        
        $RCSfile: controller.h,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:28:43 $
 *---------------------------------------------------------------------*/

#define PIFRAMSIZE		16

/*
#define	__osGetId	osPfsRepairId
*/
typedef struct {
	u32	ramarray[PIFRAMSIZE-1];		/* RAM */
	u32	pifstatus;		
} OSPifRam;

/* command type */

#define CONT_REQUEST		0
#define CONT_READ		1
#define CONT_RAM_READ		2
#define CONT_RAM_WRITE		3
#define CONT_EEPROM_READ	4
#define CONT_EEPROM_WRITE	5
#define CONT_VOICE_READ36	9
#define CONT_VOICE_WRITE20	0xa
#define CONT_VOICE_READ2	0xb
#define CONT_VOICE_WRITE4	0xc
#define CONT_VOICE_SWRITE	0xd
#define CONT_SETCH		254
#define CONT_RESET		255
#define	CONT_ETC		CONT_SETCH

#define CON_ERR_MASK		0xc0

#ifndef _HW_VERSION_1
#define CONT_FORMAT		1	
#define CHANNEL_RESET		0xfd	 
#define FORMAT_END		0xfe	 
#endif

/* request/reset format */
typedef struct {
	u8	dummy;                  /* for word aliening */
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8      typeh;
	u8      typel;
	u8      status;
	u8	dummy1;                  /* for word aliening */
} __OSContRequesFormat; 

/* request/reset format */
typedef struct {
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8      typeh;
	u8      typel;
	u8      status;
} __OSContRequesFormatShort; 

#include "controller_pfs.h"
/*#include "controller_eeprom.h"*/
#include "controller_gbpak.h"
/*#include "controller_voice.h"
#include "controller_cont.h"
#include "controller_si.h"*/

#endif /* _CONTROLLER_H_ */
