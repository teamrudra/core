#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //for handeling file discriptors
#include <linux/i2c-dev.h>
#include <math.h>
#include <fstream>
#include <string>
#include <cstdio>


using namespace std;

const int HMC5883L_I2C_ADDR = 0x1E;

#define buflen 5
#define port1 3302
#define port 3301 //The port in which the connection has to be made to connect with the server.
#define ip "192.168.1.25" //Host address has to be entered here.
#define gs "192.168.1.127"

#define SPI_CHAN                0
#define speed                   1000000

//-----------------------gps------------------------------------
string split(string str){
        int i,j;
        string lat="          ",lon="           ";
        double latf,lonf;
        lat=str.substr(17,26);
        lon=str.substr(31,40);
        lat=to_string((stof(lat)/100)+0.328529);
        lon=to_string((stof(lon)/100)+0.015334);
        str=lat+","+lon;
        return str;
}

//-------------------compass------------------------------------
void selectDevice(int fd, int addr)
{
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
        cout<<"HMC5883L not present"<<endl;
    }
}

void writeToDevice(int fd, int reg, int val)
{
    char buf[2];
    buf[0]=reg;
    buf[1]=val;

    if (write(fd, buf, 2) != 2)
    {
        cout<<"Can't write to ADXL345\n";
    }
}

int setup(){
    int fd;

    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        // Open port for reading and writing
        cout<<"Failed to open i2c bus\n";
        return 1;
    }

    /* initialise ADXL345 */

    selectDevice(fd, HMC5883L_I2C_ADDR);

    writeToDevice(fd, 0x01, 32);
    writeToDevice(fd, 0x02, 0);

    return fd;

}

int maps(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float compass(unsigned char buf[], int fd){

        buf[0] = 0x03;

        if ((write(fd, buf, 1)) != 1)
        {
            // Send the register to read from
            cout<<"Error writing to i2c slave\n";
        }

        if (read(fd, buf, 6) != 6) {
            cout<<"Unable to read from HMC5883L\n";
        } else {
            short x = (buf[0] << 8) | buf[1];
            short y = (buf[4] << 8) | buf[5];
            short z = (buf[2] << 8) | buf[3];

            float angle = atan2(y, x) * 180 / M_PI;

            if(angle>360)
                angle-=360;
            if(angle<0)
                angle+=360;
            angle=maps(angle,0,360,360,0);
            // cout<<angle<<endl;
            return angle;
        }
}

//----------------communication-----------------------------------------

void udp(int fd){
        unsigned char buf[buflen];
        for (int i = 0; i < buflen; i++)
              buf[i] = ' ';
        sockaddr_in si_me, si_other, me, other;
        int s, s1=0, i, slen=sizeof(si_other),recv_len;
        s=socket(AF_INET, SOCK_DGRAM, 0); // Creating a socket with IPv4 protocal (AF_INET) for UDP (SOCK_DGRAM) communication.
        
        s1=socket(AF_INET, SOCK_DGRAM, 0); // Creating a socket with IPv4 protocal (AF_INET) for UDP (SOCK_DGRAM) communication.
        
        if(s<0) {
                cout<<"Socket failed"<<endl;
                return;
        }
        
        memset((char *) &si_other, 0, sizeof(si_other)); //Initializing all the values of the structure to 0.
        
        
        si_me.sin_family=AF_INET; //IPv4
        si_me.sin_port=htons(port1);  //host to network short
        si_me.sin_addr.s_addr=INADDR_ANY; //Uses any address that is available.

        if(bind(s, (struct sockaddr*)&si_me,sizeof(si_me))<0) //Binding to the socket using the file discriptor
        {
                cout<<"Binding failed"<<endl;
                return;
        }

        recv_len=recvfrom(s,buf,buflen,0,(struct sockaddr*)&si_other,(socklen_t *)&slen);
        if(buf[0] == '#' && buf[4] == '$')
            cout<<buf[1]<<" "<<buf[3]<<endl;
        string str;
        fstream f;
        f.open("/dev/ttyUSB0");
        // cout<<"listening...."<<endl;
        string co;
        while(1){
            f >> str;
            if(str[5]=='A'&&str[4]=='G'){
                co=split(str);
                // cout<<co<<endl;
            }
            unsigned char buff[16];
            unsigned char buf[buflen];
            float angle=compass(buff,fd);
            // cout<<angle<<","<<co;
            string dat=co+","+to_string(angle);
            // cout<<dat<<endl;
            int dlen=dat.size();
            unsigned char* uc = (unsigned char*)dat.c_str();
            sendto(s, uc, dlen, 0, (struct sockaddr*) &si_other, slen);
        }
        close(s);
    }



int main(){
    int fd=setup();
    udp(fd);
    return 0;
}

