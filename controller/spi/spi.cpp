#include "spi.h"

Spi::Spi(int channel,int speed){
    CHANNEL = channel;
    SPEED = speed;
    if ((fd = wiringPiSPISetup (CHANNEL, SPEED)) < 0)
        cerr << "Can't open the SPI bus on channel : " << CHANNEL << endl;
}

int Spi::RW(unsigned char *data, int size) {
    return wiringPiSPIDataRW (CHANNEL, data, size);
}
