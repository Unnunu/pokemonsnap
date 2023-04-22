#ifndef _CONTROLLER_GBPAK_H_
#define _CONTROLLER_GBPAK_H_

/*---------------------------------------------------------------------*
        Copyright (C) 1998 Nintendo.
        
        $RCSfile: controller_gbpak.h,v $
        $Revision: 1.1.1.1 $
        $Date: 2002/05/02 03:28:43 $
 *---------------------------------------------------------------------*/

/* definition for 64GB-PAK */
#define PFS_BANK_ADDR   (0x8000/BLOCKSIZE)
#define GB_GBBANK_ADDR  (0xa000/BLOCKSIZE)
#define GB_POWER_ADDR   (0xb000/BLOCKSIZE)
#define OS_GBPAK_ID     0x84
#define OS_PFS_CHECK_ID 0xfe
#define GBPAK_MAX_BANK  3

#define __osGbpakRead32(pfs, address, buffer)  \
__osContRamRead((pfs)->queue, (pfs)->channel, address, buffer)

#define __osGbpakWrite32(pfs, address, buffer)  \
__osContRamWrite((pfs)->queue, (pfs)->channel, address, buffer, 0)


/* from gbpaksetbank.c */
extern s32 __osGbpakSetBank(OSPfs *, u8);

#endif /* _CONTROLLER_GBPAK_H_ */
