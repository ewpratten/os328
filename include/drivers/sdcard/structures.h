#ifndef _SDCARD_STRUCTURES_H
#define _SDCARD_STRUCTURES_H

/* Based off the Arduino Sd2Card Library */

// SD card commands
/** GO_IDLE_STATE - init card in spi mode if CS low */
#define CMD0 0X00
/** SEND_IF_COND - verify SD Memory Card interface operating condition.*/
#define CMD8 0X08
/** SEND_CSD - read the Card Specific Data (CSD register) */
#define CMD9 0X09
/** SEND_CID - read the card identification information (CID register) */
#define CMD10 0X0A
/** SEND_STATUS - read the card status register */
#define CMD13 0X0D
/** READ_BLOCK - read a single data block from the card */
#define CMD17 0X11
/** WRITE_BLOCK - write a single data block to the card */
#define CMD24 0X18
/** WRITE_MULTIPLE_BLOCK - write blocks of data until a STOP_TRANSMISSION */
#define CMD25 0X19
/** ERASE_WR_BLK_START - sets the address of the first block to be erased */
#define CMD32 0X20
/** ERASE_WR_BLK_END - sets the address of the last block of the continuous
    range to be erased*/
#define CMD33 0X21
/** ERASE - erase all previously selected blocks */
#define CMD38 0X26
/** APP_CMD - escape for application specific command */
#define CMD55 0X37
/** READ_OCR - read the OCR register of a card */
#define CMD58 0X3A
/** SET_WR_BLK_ERASE_COUNT - Set the number of write blocks to be
     pre-erased before writing */
#define ACMD23 0X17
/** SD_SEND_OP_COMD - Sends host capacity support information and
    activates the card's initialization process */
#define ACMD41 0X29

#define R1_READY_STATE 0X00
/** status for card in the idle state */
#define R1_IDLE_STATE 0X01
/** status bit for illegal command */
#define R1_ILLEGAL_COMMAND 0X04
/** start data token for read or write single block*/
#define DATA_START_BLOCK 0XFE
/** stop token for write multiple blocks*/
#define STOP_TRAN_TOKEN 0XFD
/** start data token for write multiple blocks*/
#define WRITE_MULTIPLE_TOKEN 0XFC
/** mask for data response tokens after a write block operation */
#define DATA_RES_MASK 0X1F
/** write data accepted token */
#define DATA_RES_ACCEPTED 0X05



/** init timeout ms */
#define SD_INIT_TIMEOUT 2000
/** erase timeout ms */
#define SD_ERASE_TIMEOUT 10000
/** read timeout ms */
#define SD_READ_TIMEOUT 300
/** write time out ms */
#define SD_WRITE_TIMEOUT 600

struct partitionTable {
          /**
           * Boot Indicator . Indicates whether the volume is the active
           * partition.  Legal values include: 0X00. Do not use for booting.
           * 0X80 Active partition.
           */
  char  boot;
          /**
            * Head part of Cylinder-head-sector address of the first block in
            * the partition. Legal values are 0-255. Only used in old PC BIOS.
            */
  char  beginHead;
          /**
           * Sector part of Cylinder-head-sector address of the first block in
           * the partition. Legal values are 1-63. Only used in old PC BIOS.
           */
  unsigned beginSector : 6;
           /** High bits cylinder for first block in partition. */
  unsigned beginCylinderHigh : 2;
          /**
           * Combine beginCylinderLow with beginCylinderHigh. Legal values
           * are 0-1023.  Only used in old PC BIOS.
           */
  char  beginCylinderLow;
          /**
           * Partition type. See defines that begin with PART_TYPE_ for
           * some Microsoft partition types.
           */
  char  type;
          /**
           * head part of cylinder-head-sector address of the last sector in the
           * partition.  Legal values are 0-255. Only used in old PC BIOS.
           */
  char  endHead;
          /**
           * Sector part of cylinder-head-sector address of the last sector in
           * the partition.  Legal values are 1-63. Only used in old PC BIOS.
           */
  unsigned endSector : 6;
           /** High bits of end cylinder */
  unsigned endCylinderHigh : 2;
          /**
           * Combine endCylinderLow with endCylinderHigh. Legal values
           * are 0-1023.  Only used in old PC BIOS.
           */
  char  endCylinderLow;
           /** Logical block address of the first block in the partition. */
  unsigned int firstSector;
           /** Length of the partition, in blocks. */
  unsigned int totalSectors;
};
/** Type name for partitionTable */
typedef struct partitionTable part_t;
//------------------------------------------------------------------------------
/**
 * \struct masterBootRecord
 *
 * \brief Master Boot Record
 *
 * The first block of a storage device that is formatted with a MBR.
 */
struct masterBootRecord {
           /** Code Area for master boot program. */
  char  codeArea[440];
           /** Optional WindowsNT disk signature. May contain more boot code. */
  unsigned int diskSignature;
           /** Usually zero but may be more boot code. */
  short usuallyZero;
           /** Partition tables. */
  part_t   part[4];
           /** First MBR signature byte. Must be 0X55 */
  char  mbrSig0;
           /** Second MBR signature byte. Must be 0XAA */
  char  mbrSig1;
};
/** Type name for masterBootRecord */
typedef struct masterBootRecord mbr_t;

/** 
 * \struct biosParmBlock
 *
 * \brief BIOS parameter block
 * 
 *  The BIOS parameter block describes the physical layout of a FAT volume.
 */
struct biosParmBlock {
          /**
           * Count of bytes per sector. This value may take on only the
           * following values: 512, 1024, 2048 or 4096
           */
  short bytesPerSector;
          /**
           * Number of sectors per allocation unit. This value must be a
           * power of 2 that is greater than 0. The legal values are
           * 1, 2, 4, 8, 16, 32, 64, and 128.
           */
  char  sectorsPerCluster;
          /**
           * Number of sectors before the first FAT.
           * This value must not be zero.
           */
  short reservedSectorCount;
          /** The count of FAT data structures on the volume. This field should
           *  always contain the value 2 for any FAT volume of any type.
           */
  char  fatCount;
          /**
          * For FAT12 and FAT16 volumes, this field contains the count of
          * 32-byte directory entries in the root directory. For FAT32 volumes,
          * this field must be set to 0. For FAT12 and FAT16 volumes, this
          * value should always specify a count that when multiplied by 32
          * results in a multiple of bytesPerSector.  FAT16 volumes should
          * use the value 512.
          */
  short rootDirEntryCount;
          /**
           * This field is the old 16-bit total count of sectors on the volume.
           * This count includes the count of all sectors in all four regions
           * of the volume. This field can be 0; if it is 0, then totalSectors32
           * must be non-zero.  For FAT32 volumes, this field must be 0. For
           * FAT12 and FAT16 volumes, this field contains the sector count, and
           * totalSectors32 is 0 if the total sector count fits
           * (is less than 0x10000).
           */
  short totalSectors16;
          /**
           * This dates back to the old MS-DOS 1.x media determination and is
           * no longer usually used for anything.  0xF8 is the standard value
           * for fixed (non-removable) media. For removable media, 0xF0 is
           * frequently used. Legal values are 0xF0 or 0xF8-0xFF.
           */
  char  mediaType;
          /**
           * Count of sectors occupied by one FAT on FAT12/FAT16 volumes.
           * On FAT32 volumes this field must be 0, and sectorsPerFat32
           * contains the FAT size count.
           */
  short sectorsPerFat16;
           /** Sectors per track for interrupt 0x13. Not used otherwise. */
  short sectorsPerTrtack;
           /** Number of heads for interrupt 0x13.  Not used otherwise. */
  short headCount;
          /**
           * Count of hidden sectors preceding the partition that contains this
           * FAT volume. This field is generally only relevant for media
           *  visible on interrupt 0x13.
           */
  unsigned int hidddenSectors;
          /**
           * This field is the new 32-bit total count of sectors on the volume.
           * This count includes the count of all sectors in all four regions
           * of the volume.  This field can be 0; if it is 0, then
           * totalSectors16 must be non-zero.
           */
  unsigned int totalSectors32;
          /**
           * Count of sectors occupied by one FAT on FAT32 volumes.
           */
  unsigned int sectorsPerFat32;
          /**
           * This field is only defined for FAT32 media and does not exist on
           * FAT12 and FAT16 media.
           * Bits 0-3 -- Zero-based number of active FAT.
           *             Only valid if mirroring is disabled.
           * Bits 4-6 -- Reserved.
           * Bit 7	-- 0 means the FAT is mirrored at runtime into all FATs.
	         *        -- 1 means only one FAT is active; it is the one referenced in bits 0-3.
           * Bits 8-15 	-- Reserved.
           */
  short fat32Flags;
          /**
           * FAT32 version. High byte is major revision number.
           * Low byte is minor revision number. Only 0.0 define.
           */
  short fat32Version;
          /**
           * Cluster number of the first cluster of the root directory for FAT32.
           * This usually 2 but not required to be 2.
           */
  unsigned int fat32RootCluster;
          /**
           * Sector number of FSINFO structure in the reserved area of the
           * FAT32 volume. Usually 1.
           */
  short fat32FSInfo;
          /**
           * If non-zero, indicates the sector number in the reserved area
           * of the volume of a copy of the boot record. Usually 6.
           * No value other than 6 is recommended.
           */
  short fat32BackBootBlock;
          /**
           * Reserved for future expansion. Code that formats FAT32 volumes
           * should always set all of the bytes of this field to 0.
           */
  char  fat32Reserved[12];
};
/** Type name for biosParmBlock */
typedef struct biosParmBlock bpb_t;

//------------------------------------------------------------------------------
/**
 * \struct fat32BootSector
 *
 * \brief Boot sector for a FAT16 or FAT32 volume.
 * 
 */  
struct fat32BootSector {
           /** X86 jmp to boot program */
  char  jmpToBootCode[3];
           /** informational only - don't depend on it */
  char     oemName[8];
           /** BIOS Parameter Block */
  bpb_t    bpb;
           /** for int0x13 use value 0X80 for hard drive */
  char  driveNumber;
           /** used by Windows NT - should be zero for FAT */
  char  reserved1;
           /** 0X29 if next three fields are valid */
  char  bootSignature;
           /** usually generated by combining date and time */
  unsigned int volumeSerialNumber;
           /** should match volume label in root dir */
  char     volumeLabel[11];
           /** informational only - don't depend on it */
  char     fileSystemType[8];
           /** X86 boot code */
  char  bootCode[420];
           /** must be 0X55 */
  char  bootSectorSig0;
           /** must be 0XAA */
  char  bootSectorSig1;
};
//------------------------------------------------------------------------------
// End Of Chain values for FAT entries
/** FAT16 end of chain value used by Microsoft. */
#define FAT16EOC 0XFFFF
/** Minimum value for FAT16 EOC.  Use to test for EOC. */
#define FAT16EOC_MIN 0XFFF8;
/** FAT32 end of chain value used by Microsoft. */
#define FAT32EOC 0X0FFFFFFF;
/** Minimum value for FAT32 EOC.  Use to test for EOC. */
#define FAT32EOC_MIN 0X0FFFFFF8;
/** Mask a for FAT32 entry. Entries are 28 bits. */
#define FAT32MASK 0X0FFFFFFF;

/** Type name for fat32BootSector */
typedef struct fat32BootSector fbs_t;

/**
 * \struct directoryEntry
 * \brief FAT short directory entry
 *
 * Short means short 8.3 name, not the entry size.
 *  
 * Date Format. A FAT directory entry date stamp is a 16-bit field that is 
 * basically a date relative to the MS-DOS epoch of 01/01/1980. Here is the
 * format (bit 0 is the LSB of the 16-bit short, bit 15 is the MSB of the 
 * 16-bit short):
 *   
 * Bits 9-15: Count of years from 1980, valid value range 0-127 
 * inclusive (1980-2107).
 *   
 * Bits 5-8: Month of year, 1 = January, valid value range 1-12 inclusive.
 *
 * Bits 0-4: Day of month, valid value range 1-31 inclusive.
 *
 * Time Format. A FAT directory entry time stamp is a 16-bit field that has
 * a granularity of 2 seconds. Here is the format (bit 0 is the LSB of the 
 * 16-bit short, bit 15 is the MSB of the 16-bit short).
 *   
 * Bits 11-15: Hours, valid value range 0-23 inclusive.
 * 
 * Bits 5-10: Minutes, valid value range 0-59 inclusive.
 *      
 * Bits 0-4: 2-second count, valid value range 0-29 inclusive (0 - 58 seconds).
 *   
 * The valid time range is from Midnight 00:00:00 to 23:59:58.
 */
struct directoryEntry {
           /**
            * Short 8.3 name.
            * The first eight bytes contain the file name with blank fill.
            * The last three bytes contain the file extension with blank fill.
            */
  char  name[11];
          /** Entry attributes.
           *
           * The upper two bits of the attribute byte are reserved and should
           * always be set to 0 when a file is created and never modified or
           * looked at after that.  See defines that begin with DIR_ATT_.
           */
  char  attributes;
          /**
           * Reserved for use by Windows NT. Set value to 0 when a file is
           * created and never modify or look at it after that.
           */
  char  reservedNT;
          /**
           * The granularity of the seconds part of creationTime is 2 seconds
           * so this field is a count of tenths of a second and its valid
           * value range is 0-199 inclusive. (WHG note - seems to be hundredths)
           */
  char  creationTimeTenths;
           /** Time file was created. */
  short creationTime;
           /** Date file was created. */
  short creationDate;
          /**
           * Last access date. Note that there is no last access time, only
           * a date.  This is the date of last read or write. In the case of
           * a write, this should be set to the same date as lastWriteDate.
           */
  short lastAccessDate;
          /**
           * High short of this entry's first cluster number (always 0 for a
           * FAT12 or FAT16 volume).
           */
  short firstClusterHigh;
           /** Time of last write. File creation is considered a write. */
  short lastWriteTime;
           /** Date of last write. File creation is considered a write. */
  short lastWriteDate;
           /** Low short of this entry's first cluster number. */
  short firstClusterLow;
           /** 32-bit unsigned holding this file's size in bytes. */
  unsigned int fileSize;
};
//------------------------------------------------------------------------------
// Definitions for directory entries
//
/** Type name for directoryEntry */
typedef struct directoryEntry dir_t;



#endif