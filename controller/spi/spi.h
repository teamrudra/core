#ifndef __SPI_H__
#define __SPI_H__
#include <wiringPiSPI.h>
#include <iostream>

using namespace std;

class Spi {
    private:
        int CHANNEL, SPEED, fd;
    public:
        Spi(int channel,int speed);
        int RW(unsigned char *data, int size);
};

#endif
