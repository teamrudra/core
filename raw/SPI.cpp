#include <wiringPiSPI.h>
#include <iostream>
#include "SPI.h"

using namespace std;

Spi::Spi(int channel,int speed){
  CHANNEL = channel;
  SPEED = speed;
  if ((SPI_fd = wiringPiSPISetup (CHANNEL, SPEED)) < 0)
      cerr << "Can't open the SPI bus on channel : " << CHANNEL << endl;
}

int Spi::RW(unsigned char *data, int size) {
    if ((SPI_data = wiringPiSPIDataRW (CHANNEL, data, size)) < 0) {
        cerr << "SPI failure on channel : " << CHANNEL << endl;
    }
    return SPI_data;
}
