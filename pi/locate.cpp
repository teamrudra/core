#include <iomanip>
#include <ctime>
#include <sstream>
#include <libgpsmm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <cmath>

using namespace std;

const int HMC5883L_I2C_ADDR = 0x1E;
gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

int gpsdintialise(){
  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
    cerr << "No GPSD running.\n";
    return 0;
  }
  return 1;
}

void get_latlon(double &latitude,double &longitude){
  struct gps_data_t *gpsd_data;

  if (!gps_rec.waiting(1000000)){
    //Do nothing
  }
  else{
    if ((gpsd_data = gps_rec.read()) == NULL) {
      cerr << "GPSD read error.\n";
    } else {
      while (((gpsd_data = gps_rec.read()) == NULL) ||
             (gpsd_data->fix.mode < MODE_2D)) {
        // Do nothing until fix
      }
      latitude  = gpsd_data->fix.latitude;
      longitude = gpsd_data->fix.longitude;
  //      double heading   = gpsd_data->fix.heading;
    }
  }
}

//================================COMPASS===================================
int maps(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void selectDevice(int fd, int addr)
{
    if (ioctl(fd, I2C_SLAVE, addr) < 0)
    {
       cout<<"HMC5883L not present"<<endl;
    }
}

void writeToDevice(int fd, int reg, int value)
{
    char buf[2];
    buf[0]=reg;
    buf[1]=value;

    if (write(fd, buf, 2) != 2)
    {
       cout<<"Can't write to ADXL345\n";
    }
}

int compass_setup(){
    int fd;
    if ((fd = open("/dev/i2c-1", O_RDWR)) < 0)
    {
        //Open port for reading and writing
        cout<<"Failed to open i2c bus\n";
        return 1;
    }
    /* initialise ADXL345 */
    selectDevice(fd, HMC5883L_I2C_ADDR);
    writeToDevice(fd, 0x01, 32);
    writeToDevice(fd, 0x02, 0);
    return fd;
}

float compass(unsigned char buf[], int fd){
	float angle;
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

           angle = atan2(y, x) * 180 / M_PI;
           angle += 70;
           if(angle>360) angle-=360;
           if(angle<0) angle+=360;

           angle=maps(angle,0,360,360,0);
       }
   return angle;
}

int main(){
  double latitude , longitude;
  int gpsdcheck =  gpsdintialise();
  int compass_fd = compass_setup();
  unsigned char buff[16];
  while(1){
    float angle = compass(buff,compass_fd);
    cout<<"Compass: "<<angle<<endl;
    get_latlon(latitude,longitude);
    cout.precision(6);
    cout.setf(std::ios::fixed, std::ios::floatfield);
    cout<<latitude << "," <<longitude << '\n';
  }
  return 0;
}
