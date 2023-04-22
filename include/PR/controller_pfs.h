#ifndef _CONTROLLER_PFS_H_
#define _CONTROLLER_PFS_H_

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
        
        $RCSfile: controller_pfs.h,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:28:43 $
 *---------------------------------------------------------------------*/

#if	0
#define _PFS_1M_EXTENSION	1	/* for New 1M-PAK lib */
#endif

#define PFS_ONE_PAGE		8
#define PFS_PAGE_SIZE		(BLOCKSIZE*PFS_ONE_PAGE)

#define PFS_INODE_SIZE_PER_PAGE	128
#define PFS_SIZE_LOC  		24
#define DEF_DIR_PAGES		2

#define PFS_ID_0AREA		1
#define PFS_ID_1AREA		3
#define PFS_ID_2AREA		4
#define PFS_ID_3AREA		6
#define PFS_LABEL_AREA		7
#define PFS_ID_PAGE     	PFS_ONE_PAGE * 0

#define PFS_WRITTEN		2

/* definition for page usage */
#define PFS_EOF			1
#define PFS_PAGE_NOT_EXIST	2
#define PFS_PAGE_NOT_USED	3

/* definition for memorypack size */
#define PFS_32K_RAM		1
#define PFS_64K_RAM		2

#define PFS_FORCE		1
#define PFS_DELETE		1

/* definition for bank */
#define	PFS_ID_BANK_256K	0
#define	PFS_ID_BANK_1M		4
#define	PFS_BANKS_256K		1


/* ram read/write format */
typedef struct {
	u8	dummy;                  /* for word aliening */
	u8	txsize;			/* Tx size */
	u8	rxsize;			/* Rx size */
	u8	cmd;			/* commmand */
	u8	addrh;
	u8	addrl;
	u8      data[BLOCKSIZE];
	u8      datacrc;
} __OSContRamReadFormat; 

typedef union {
	struct {
		u8	bank;
		u8	page;
	}inode_t;
	u16	ipage;
} __OSInodeUnit;

typedef struct {
	u32 		game_code;
	u16     	company_code;
	__OSInodeUnit   start_page;
	u8      	status;
	s8		reserved;
	u16     	data_sum;
	u8	    	ext_name[PFS_FILE_EXT_LEN];
	u8	    	game_name[PFS_FILE_NAME_LEN];
} __OSDir;

typedef struct {
	__OSInodeUnit	inode_page[PFS_INODE_SIZE_PER_PAGE];
} __OSInode;

typedef struct {
	u32	repaired;
	u32	random;
	u64	serial_mid;
	u64	serial_low;
	u16	deviceid;
	u8	banks;
	u8	version;
	u16	checksum;
	u16	inverted_checksum;
} __OSPackId;

/***** BY YASU */
#define	PFS_BANK_LAPPED_BY	8	/* => u8 */
#define	PFS_SECTOR_PER_BANK	32
#define	PFS_INODE_DIST_MAP	(PFS_BANK_LAPPED_BY * PFS_SECTOR_PER_BANK)
#define	PFS_SECTOR_SIZE		(PFS_INODE_SIZE_PER_PAGE/PFS_SECTOR_PER_BANK)

typedef	struct {
  __OSInode	inode;
  u8		bank;
  u8		map[PFS_INODE_DIST_MAP];
} __OSInodeCache;

extern OSPifRam		__osPfsPifRam;		/* Memory Pack buffer */
extern int		__osPfsLastChannel;
extern u8		__osPfsInodeCacheBank;
extern __OSInode	__osPfsInodeCache;
extern int		__osPfsInodeCacheChannel;


/* from contramread.c */
extern s32 __osContRamRead(OSMesgQueue *, int, u16, u8 *);

/* from contramwrite.c */
extern s32 __osContRamWrite(OSMesgQueue *, int, u16, u8 *, int);

/* from contpfs.c */
extern u16 __osSumcalc(u8 *,int);
extern s32 __osIdCheckSum(u16 *, u16 *, u16 *);
extern s32 __osCheckPackId(OSPfs *, __OSPackId *);
extern s32 __osGetId(OSPfs *);
extern s32 __osCheckId(OSPfs *);
extern s32 __osPfsRWInode(OSPfs *, __OSInode *, u8, u8);
extern s32 __osPfsSelectIdBank(OSPfs *);
extern s32 __osRepairPackId(OSPfs *, __OSPackId *, __OSPackId *);

/* from pfsgetstatus.c */
extern s32 __osPfsGetStatus(OSMesgQueue *, int);
extern void __osPfsRequestOneChannel(int, u8);
extern void __osPfsGetOneChannelData(int, OSContStatus *);

/* from pfsallocatefile.c */
extern s32 __osPfsDeclearPage(OSPfs *, __OSInode *, int, int *, u8,int *,int *);

/* from pfsdeletefile.c */
extern s32 __osPfsReleasePages(OSPfs *, __OSInode *,u8,u8, __OSInodeUnit *);

/* from pfsisplug.c */
extern void __osPfsRequestData(u8 );
extern void __osPfsGetInitData(u8 *, OSContStatus *);

/* from pfsselectbank.c */
extern s32 __osPfsSelectBank(OSPfs *, u8);


#endif /* _CONTROLLER_PFS_H_ */
