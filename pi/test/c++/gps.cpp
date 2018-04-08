#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <libgpsmm.h>

using namespace std;
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

int main(void)
{
  double latitude , longitude;
  int gpsdcheck =  gpsdintialise();
  while(gpsdcheck) {
    get_latlon(latitude,longitude);
    cout.precision(6);
    cout.setf(std::ios::fixed, std::ios::floatfield);
    cout<<latitude << "," <<longitude << '\n';
  }
  return 0;
}

