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
#include <math.h>

using namespace std;

#define lat 12.82187
#define lon 80.63217

const int HMC5883L_I2C_ADDR = 0x1E;
gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

//==================================GPS=====================================
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

//=====================bearing========================
float get_bearing(float lat1, float lon1, float lat2, float lon2)
{
  //from the Haversine formula
  float y = sin(lon2-lon1)*cos(lat2);
  float x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
  float bearing = atan2(y, x);

  return bearing;
}

//====================Distance========================
float get_dist(float lat1, float lon1, float lat2, float lon2)
{
  //from the Haversine formula
  double r = 6371000;                      //radius of the earth in meters (6371km)
  float dlat = lat2 - lat1;
  float dlon = lon2 - lon1;
  float a = sin(dlat/2) * sin(lat/2) + cos(lat1) * cos(lat2) * sin(dlon/2) * sin(dlon/2);
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  float d = r * c;

  return d;
}


//====================Auto============================
void auto_bot(float bear, float dist, float head)
{
  float diff = head - bear;
  diff = diff>0?diff:-diff;
  // if(abs(diff)>180)
  //   diff = (360-abs(diff))*diff/abs(diff);
  cout<<"Diff"<<diff<<endl;
  if(diff > 5 && diff < 180)
  {
    //left
    cout<<"left"<<endl;
  } 
  else if(diff >=180 && diff < 355)
  {
    //right
    cout<<"right"<<endl;
  }
  else if((diff >= 0 && diff <= 5)||(diff >= 355 && diff <= 360))
  {
    if(dist > 3)
    {
      //forward
      cout<<"forward"<<endl;
    } 
  }
  else 
  {
    //stop
    cout<<"stop"<<endl;
  }


}

//======================Setup=========================
int setup()
{
  int gpsdcheck =  gpsdintialise();
  int compass_fd = compass_setup();

  return compass_fd;
}

//======================main==========================
int main(){
  double latitude , longitude;
  unsigned char buff[16];
  int compass_fd = setup();
  while(1){
 
    float angle = compass(buff,compass_fd);
    cout<<"Compass: "<<angle<<endl;
 
    get_latlon(latitude,longitude);
    cout.precision(6);
    cout.setf(std::ios::fixed, std::ios::floatfield);
 
    float bear = get_bearing(lat, lon, latitude, longitude);
    float dist = get_dist(lat, lon, latitude, longitude);

    auto_bot(bear, dist, angle);

    cout<< latitude << "," << longitude << ","<< bear << "," << dist <<endl;

  }
  return 0;
}