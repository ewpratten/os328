#ifndef _SDCARD_SDCARD_H
#define _SDCARD_SDCARD_H
/**
 * 
 */
 
#include "structures.h"

#define SD_CARD_SS_PIN 4 

struct SDCartInfo {  
  unsigned int volumeStartSector;
  short sectorsPerFat;
  char  sectorsPerCluster;
  unsigned int fatStartSector;   
  unsigned int rootDirSector;
  unsigned int dataStartSector;
  unsigned int fatSector;
  char  csPin;
};

struct SDFile {  
  short cursorInSector;
  unsigned int currentSector;
  unsigned int currentCluster;
  char  sectorCounter;
  unsigned int length;
  unsigned int filePosition;
};

extern unsigned char  lastStatus;
extern struct SDFile         file;
extern struct SDCartInfo     sdCardInfo;
extern unsigned char  sdBuffer[512];
extern unsigned short fatBuffer[256];


// extern long const FAT32MASK;

#define BUFFER_LENGTH 512


void sd_selectSDCard();

void sd_deselectedSDCard();


unsigned char sd_doSDCardCommand(unsigned char cmd, unsigned int arg);

unsigned char sd_doASCSDCardCommand(unsigned char cmd, unsigned int arg);


int sd_initSDCard( void );


int sd_readSectorFromSDCard(unsigned int sector, unsigned char* dst );


int sd_readSDCardInfos();

unsigned int sd_cluster2Sector( unsigned int cluster );

int sd_openFile( dir_t *f );

int sd_firstRootDirEntry();

int sd_nextRootDirEntry();

int sd_nextRootDirEntryAvailable();

int sd_dataAvailable();

int sd_selectNextCluster();

unsigned char sd_readByte();

int sd_sdCardBegin( int csPin );

#endif