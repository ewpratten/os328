#include "../../include/drivers/spi/spi.h"
#include "../../include/drivers/sdcard/sdcard.h"
#include "../../include/io/gpio.h"
#include "../../include/drivers/arduino/millis.h"

#define true 1
#define false 0


unsigned char       lastStatus;
struct SDFile        file;
struct SDCartInfo    sdCardInfo;
unsigned char          sdBuffer[512];
unsigned short      fatBuffer[256];

/**
 * SD-Karte auswählen
 */
void sd_selectSDCard() {
    gpio_write(sdCardInfo.csPin, LOW);
}

/**
 * Auswahl aufheben
 */
void sd_deselectedSDCard() {  
    gpio_write(sdCardInfo.csPin, HIGH);
}

/**
 * Sende einen SD-Card-Befehl
 * cmd der Befehl
 * arg der Parameter
 *
 * Wartet das Ergebnis ab und liefert es zurück
 */
unsigned char sd_doSDCardCommand(unsigned char cmd, unsigned int arg) {

  unsigned char result= 0x00;

  // send command
  spi_write(cmd | 0x40, 1);

  // send argument
  for (int8_t s = 24; s >= 0; s -= 8) 
    spi_write(arg >> s, 1);

  // send CRC
  unsigned char crc = 0XFF;
  if (cmd == CMD0) 
    crc = 0X95;  // correct crc for CMD0 with arg 0
  if (cmd == CMD8) 
    crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
  spi_write(crc, 1);

  // wait for response
  for (unsigned char i = 0; ((result = spi_write(0xFF, 1)) & 0x80) && i != 0xFF; i++);
  
  lastStatus= result;
  
  return result;
}

/**
 * Sende einen ASC-Command (escape for application specific command)
 */
unsigned char sd_doASCSDCardCommand(unsigned char cmd, unsigned int arg) {
    sd_doSDCardCommand(CMD55, 0);
    return sd_doSDCardCommand(cmd, arg);
}

/**
 * Initialisierungsroutine für die SD-Karte
 */
int sd_initSDCard( void ) {
  
  unsigned char result= 0x00;
  
  // Aktuell haben wir keinen FAT-Sektor geladen. Das markieren wir hiermit:
  sdCardInfo.fatSector= 0;
  
  // unter 400khz setzen
//   SPI.setClockDivider( SPI_CLOCK_DIV128 );

  // ca. 74 Zyklen warten
  for (unsigned char i = 0; i < 10; i++) 
    spi_write(0xFF, 1);
  
  sd_selectSDCard();
  
  // CMD0 mit timeout aufrufen
  unsigned int t0 = millis();
  
  while( (result = sd_doSDCardCommand(CMD0, 0)) != R1_IDLE_STATE) {
    if( (millis() - t0) > SD_INIT_TIMEOUT ) {
      break;
    } // if 
  } // while
  
  // alles ok, dann ACMD41 aufruf, womit die Karte initialisiert wird.
  if( result == R1_IDLE_STATE) {    
    unsigned int t0 = millis();
    while ((result = sd_doASCSDCardCommand(ACMD41, 0)) != R1_READY_STATE) {
      // check for timeout
      if( (millis() - t0) > SD_INIT_TIMEOUT ) {
        break;
      } // if 
    } // whilte
  } // if 
  
  sd_deselectedSDCard();  

//   SPI.setClockDivider( SPI_CLOCK_DIV4 );  

  return result == R1_READY_STATE;
}

/**
 * Liefert einen Sektor aus
 * sector: Sektornummer
 * dst   : Zielbuffer
 * len   : Länge, sollte zum Zielbuffer passen
 */
int sd_readSectorFromSDCard(unsigned int sector, unsigned char* dst ) {
  
  int result       = false;
  unsigned char  cardValue= 0x00;
  unsigned int  sdAddr  = sector * 512; // sector << 9;

  sd_selectSDCard();
  
  unsigned int t0= millis();
  while( (cardValue = sd_doSDCardCommand(CMD17, sdAddr)) != R1_READY_STATE) {
    if( (millis() - t0) > SD_READ_TIMEOUT ) {
      break;
    } // if 
  } // while
    
  // Block lesen
  if( cardValue == R1_READY_STATE ) {
    
    // warte bis daten vorliegen...
    unsigned int t0 = millis();     
    while((cardValue = spi_write(0xFF, 1)) == 0xFF) {
      if( (millis() - t0) > SD_READ_TIMEOUT) {
        break;
      } // if 
    } // while
  } // if
  
  // liegen die Daten nun vor?
  if( cardValue == DATA_START_BLOCK) {      

    unsigned short i= 0;
    for( i= 0; i < 512; i++ ) {
      dst[i] = spi_write(0xFF, 1);
    } // while

    // skip crc
    spi_write(0xFF, 1);
    spi_write(0xFF, 1);
    result= true;
  } // if 
  else
    result= false;

  sd_deselectedSDCard();  
  
  return result;
}

/**
 * Lese alle notwendigen Informationen über die SD-Karte.
 */
int sd_readSDCardInfos() {

  int result= false;
  // MBS lesen
  if( sd_readSectorFromSDCard( 0, sdBuffer ) ) {
    
    mbr_t *first= (mbr_t*)sdBuffer;   
    // merke den logischen Startsektor, diesen lesen wir nun.
    sdCardInfo.volumeStartSector = first->part[0].firstSector;   
    if( sd_readSectorFromSDCard( sdCardInfo.volumeStartSector, sdBuffer ) ) {        
      
      fbs_t *fbs= (fbs_t*)sdBuffer;
      bpb_t *bpb = &fbs->bpb;
        
      sdCardInfo.sectorsPerCluster = bpb->sectorsPerCluster;
      sdCardInfo.sectorsPerFat     = bpb->sectorsPerFat16 ? bpb->sectorsPerFat16 : bpb->sectorsPerFat32;
      sdCardInfo.fatStartSector    = sdCardInfo.volumeStartSector + bpb->reservedSectorCount;
      sdCardInfo.rootDirSector     = sdCardInfo.fatStartSector + bpb->fatCount * sdCardInfo.sectorsPerFat;
      sdCardInfo.dataStartSector   = sdCardInfo.rootDirSector + ((32 * bpb->rootDirEntryCount + 511)/512);
      result= true;
    } // if
  } // if 
  
  return result;
}

/**
 * Berechnet aus einem Cluster den Sektor.
 */
unsigned int sd_cluster2Sector( unsigned int cluster ) {
  
  unsigned int result= cluster - 2;
  result = result * sdCardInfo.sectorsPerCluster;
  result = result + sdCardInfo.dataStartSector;
        
  return result;
}

/**
 * Öffnet eine Datei
 */
int sd_openFile( dir_t *f ) {  
  file.cursorInSector= 0;
  file.filePosition  = 0;
  file.length        = f->fileSize;
  file.sectorCounter = 0;
  file.currentCluster= f->firstClusterLow;
  file.currentSector = sd_cluster2Sector( file.currentCluster );  
  return sd_readSectorFromSDCard( file.currentSector, sdBuffer );
}

/**
 * Ersten Verzeichniseintrag aus dem Root-Verzeichnis laden
 */
int sd_firstRootDirEntry() {
  file.currentSector = sdCardInfo.rootDirSector;  
  return sd_readSectorFromSDCard( file.currentSector, sdBuffer );
}

/**
 * Nächsten Eintrag laden
 */
int sd_nextRootDirEntry() {  
  file.currentSector++;  
  return sd_readSectorFromSDCard( file.currentSector, sdBuffer );
}

/**
 * Prüfung, ob das Ende des Root-Verzeichnis erreicht wurde.
 */
int sd_nextRootDirEntryAvailable() {
  return file.currentSector + 1 < sdCardInfo.dataStartSector;
}

/**
 * Haben wir das Ende erreicht?
 */
int sd_dataAvailable() {
  return file.filePosition < file.length;
}

/**
 * wähle den nächsten Cluster aus und lade den Sektor
 */
int sd_selectNextCluster() {
  
  int result         = true;
  unsigned short nextCluster= file.currentCluster; // * 2;
  unsigned short sector     = nextCluster >> 8; // / 256;
  unsigned short offset     = nextCluster & 0xFF; //% 256;

  if( sdCardInfo.fatStartSector + sector != sdCardInfo.fatSector ) {
    sdCardInfo.fatSector= sdCardInfo.fatStartSector + sector;
    result= sd_readSectorFromSDCard( sdCardInfo.fatSector, fatBuffer );
  } // if 

  if( result ) {
    file.currentCluster= fatBuffer[offset];
    file.currentSector = sd_cluster2Sector( file.currentCluster );      
    file.sectorCounter = 0;
    result             = sd_readSectorFromSDCard( file.currentSector, sdBuffer );      
  } // if 
  
  return result;
}

unsigned char sd_readByte() {
  
  if( file.cursorInSector >= BUFFER_LENGTH ) {        
    file.cursorInSector= 0;
    file.sectorCounter+= 1;
    if( file.sectorCounter >= sdCardInfo.sectorsPerCluster ) {
      sd_selectNextCluster();
    }
    else {
      file.currentSector+= 1;
      sd_readSectorFromSDCard( file.currentSector, sdBuffer );
    } // else
  } // if 
  
  file.filePosition++;
  
  return sdBuffer[file.cursorInSector++];
}

int sd_sdCardBegin( int csPin ) {

  // set the slaveSelectPin as an output:
  gpio_mode( csPin, OUTPUT);  
  gpio_write( csPin, HIGH );  
  sdCardInfo.csPin= csPin;
  spi_init();   
  return sd_initSDCard() && sd_readSDCardInfos();  
}


