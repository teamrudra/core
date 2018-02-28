#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <stdio.h>
using namespace std;

#define SPI_CHAN                 0
#define SPI_CHAN1                1
#define speed                   1000000


void spiSetup(int spi){
  if ( wiringPiSPISetup (spi, speed) < 0){
    fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
    exit(0);
  }
}

int getdata(int spi,unsigned char *data,int size){
   int d = wiringPiSPIDataRW (spi, data, size);
   if (d == -1){
      printf ("SPI failure: %s\n", strerror (errno));
      exit(0);
    }
  return d;
}


int main(){
  wiringPiSetup () ;
  spiSetup(SPI_CHAN);
  spiSetup(SPI_CHAN1);
  while (1){
    unsigned char d[] = {'1'};
    unsigned char d1[] = {'5'};
    unsigned char d2[] = {'5'};
    int c = getdata(SPI_CHAN,d,1);
    int e = getdata(SPI_CHAN1,d1,1);
     e = getdata(SPI_CHAN1,d2,1);
    usleep(10);
    cout<<d<<" ";
    cout<<d1<<endl;
  }
  return 0;
}

