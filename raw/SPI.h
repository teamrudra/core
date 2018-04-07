#ifndef SPI_H
#define SPI_H

class Spi
{
  private: int CHANNEL;
  private: int SPEED;
  private: int SPI_fd,SPI_data;
  public: Spi(int channel,int speed);
  public: int RW(unsigned char *data, int size);
  public: ~Spi();
};

#endif
