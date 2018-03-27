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

#define SPI_CHAN                1
#define speed                   10000


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
  while (1){
    unsigned char d[] = {'6'};
//    unsigned char d1[] = {'1'};
    int c = getdata(SPI_CHAN,d,1);
   // usleep(500);
  //  int w = getdata(SPI_CHAN,d1,1);
    //int a = getdata(SPI_CHAN,d1,1);
//    cout<<c<<" "<<w<<endl;
    cout<<c<<endl;
  }
  return 0;
}
