#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <libgpsmm.h>

using namespace std;

int main(void)
{
  gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);
  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
    cerr << "No GPSD running.\n";
    return 1;
  }

  while(1) {
    struct gps_data_t *gpsd_data;

    if (!gps_rec.waiting(1000000)) continue;

    if ((gpsd_data = gps_rec.read()) == NULL) {
      cerr << "GPSD read error.\n";
      return 1;
    } else {
      while (((gpsd_data = gps_rec.read()) == NULL) ||
             (gpsd_data->fix.mode < MODE_2D)) {
	// Do nothing until fix
      }
      double latitude  = gpsd_data->fix.latitude;
      double longitude = gpsd_data->fix.longitude;
//      double heading   = gpsd_data->fix.heading;
       cout.precision(6);
       cout.setf(std::ios::fixed, std::ios::floatfield);
       cout<<latitude << "," <<longitude << '\n';
    }
  }
  return 0;
}
