#ifndef _DRIVERS_SPI_H
#define _DRIVERS_SPI_H

void spi_init();

int spi_read(char *buf, int len);
int spi_write(char *buf, int len);

void spi_ioctl_select(int dev);
void spi_ioctl_release(int dev);

#endif /* _DRIVERS_SPI_H */