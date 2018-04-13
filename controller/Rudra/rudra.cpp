#include "rudra.h"

gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

Rudra::Rudra(){
}

int Rudra::gpsdintialise(){
  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
    cerr << "No GPSD running.\n";
    return 0;
  }
  return 1;
}

void Rudra::get_latlon(double &latitude,double &longitude){
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

double Rudra::maps(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Rudra::get_bearing(float lat1, float lon1, float lat2, float lon2)
{
  //from the Haversine formula
  lat1 *= d2r;
  lon1 *= d2r;
  lat2 *= d2r;
  lon2 *= d2r;

  double y = sin(lon2-lon1)*cos(lat2);
  double x = cos(lat1)*sin(lat2) - sin(lat1)*cos(lat2)*cos(lon2-lon1);
  double bearing = atan2(y, x)*(r2d);
  if(bearing < 0 ) bearing = 360 + bearing;
  return bearing;
}

float Rudra::get_dist(float lat1, float lon1, float lat2, float lon2)
{
  //from the Haversine formula
  lat1 *= d2r;
  lat2 *= d2r;
  double d_phi = (lat2 - lat1) / 2;
  double d_lam = ((lon2 - lon1) * d2r ) / 2;
  double a = (sin(d_phi) * sin(d_phi)) + (cos(lat1) * cos(lat2) * sin(d_lam) * sin(d_lam));
  return (12742000 * atan2(sqrt(a), sqrt(1-a))); // meters
}

unsigned char Rudra::parse(unsigned char *n, int start, int end) {
    int a = 0;
    for (int i = start; i<end; i++) {
        a += (n[i]-'0');
        a *= 10;
    }
    return (unsigned char)(a/10);
}

