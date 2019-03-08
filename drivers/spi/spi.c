

#include <inttypes.h>
#include <avr/io.h>


#define DDR_SPI		DDRB
#define PORT_SPI	PORTB
#define DD_MOSI		3
#define DD_SCK		5
/* fuck you, Atmel! You will be burn in the Hell! (without this pin not worked) */
#define DD_SS		2

/* SPI mode settings */
#define SPI_MODE_MASTER	1
#define SPI_CPOL	0
#define SPI_CPHA	0

#define SPI_CLK_DIV4	0
#define SPI_CLK_DIV16	1
#define SPI_CLK_DIV64	2
#define SPI_CLK_DIV128	3

#define SPI_CLK_DIVIDER_VALUE SPI_CLK_DIV64
#define SPI_DOUBLE_SPEED 1 /* SPI speed * 2 */

static void avr_spi_master_init()
{
	/* Set MOSI, SCK and SS as output */
	DDR_SPI = _BV(DD_MOSI) | _BV(DD_SCK) | _BV(DD_SS);

	/* Setup SPI control register */
	SPCR = _BV(SPE) /* SPI enable */
	| (SPI_MODE_MASTER ? _BV(MSTR) : 0)
	| (SPI_CPOL ? _BV(CPOL) : 0)
	| (SPI_CPHA ? _BV(CPHA) : 0)
	| SPI_CLK_DIVIDER_VALUE;
	
	SPSR = SPI_DOUBLE_SPEED ? _BV(SPI2X) : 0;
}

static uint8_t avr_spi_master_exchange(uint8_t c)
{
	/* Start transaction */
	SPDR = c;
	/* Wait for transaction complete */
	while(!(SPSR & _BV(SPIF)));
	return SPDR;
}

static void avr_spi_slave_select(int dev)
{
	PORT_SPI &= ~_BV(DD_SS);
}

static void avr_spi_slave_release(int dev)
{
	PORT_SPI |= _BV(DD_SS);
}


void spi_init()
{
	avr_spi_master_init();
}

int spi_read(char *buf, int len)
{
	int i = 0;
	while(len--)
		buf[i++] = avr_spi_master_exchange(0);
	return i;
}

int spi_write(char *buf, int len)
{
	int i = 0;
	while(len--)
		avr_spi_master_exchange(buf[i++]);
	return i;
}

void spi_ioctl_select(int dev)
{
	avr_spi_slave_select(dev);
}

void spi_ioctl_release(int dev)
{
	avr_spi_slave_release(dev);
}
