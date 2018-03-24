#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //for handeling file discriptors

using namespace std;

#define buflen 5
#define port 3301 //The port in which the connection has to be made to connect with the server.
#define ip "192.168.1.47" //Host address has to be entered here.

#define SPI_CHAN1               1
#define SPI_CHAN                0
#define speed                   1000000

void spiSetup(int spi){
  if ( wiringPiSPISetup (spi, speed) < 0){
    fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
    exit(0);
  }
}

int getdata(int spi,unsigned char *data,int size,int pin){
   int d = wiringPiSPIDataRW (spi, data, size);
   if (d == -1){
      printf ("SPI failure: %s\n", strerror (errno)) ;
      exit(0);
    }
  return d;
}

void udp(){
        unsigned char buf[buflen];
        for (int i = 0; i < buflen; i++)
              buf[i] = ' ';
        sockaddr_in si_me, si_other;
        int s, i, slen=sizeof(si_other),recv_len;
        s=socket(AF_INET, SOCK_DGRAM, 0); // Creating a socket with IPv4 protocal (AF_INET) for UDP (SOCK_DGRAM) communication.
        if(s<0) {
                cout<<"Socket failed"<<endl;
                return;
        }
        memset((char *) &si_other, 0, sizeof(si_other)); //Initializing all the values of the structure to 0.
        si_me.sin_family=AF_INET; //IPv4
        si_me.sin_port=htons(port);  //host to network short
        si_me.sin_addr.s_addr=INADDR_ANY; //Uses any address that is available.
        if(bind(s, (struct sockaddr*)&si_me,sizeof(si_me))<0) //Binding to the socket using the file discriptor
        {
                cout<<"Binding failed"<<endl;
                return;
        }
        cout<<"listening.."<<endl;
        while(1){
                unsigned char buf[buflen];
                recv_len=recvfrom(s,buf,buflen,0,(struct sockaddr*)&si_other,(socklen_t *)&slen);
//              cout<<buf<<" ";
                if(buf[0] == '#' && buf[4] == '$'){
                        cout<<buf[1]<<" "<<buf[3]<<endl;
                        getdata(SPI_CHAN,&buf[1],1,1);
//                        getdata(SPI_CHAN,&buf[3],1,1);
                }
        }
        close(s);
}


int main(){
        spiSetup(SPI_CHAN);
//        spiSetup(SPI_CHAN1);
        udp();
        return 0;
}

