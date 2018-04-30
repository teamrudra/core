#include"helper/helper.h"
#include"udp/udp.h"
#include <iostream>
#include "hmc5883l/hmc5883l.h"

using namespace std;

#define port 23907

Helper H;
Udp U(port);
HMC5883L compass;

int bytes;

double latitude,longitude;
double heading;

if(!H.gpsdintialise())
void setup(){
     cout<<"ERROR: initialising gpsd! "<<endl;
  if( hmc5883l_init(&compass) != HMC5883L_OKAY )
      cout << "ERROR: initialising compass!" << endl;
}

double Compass(double heading){
  heading = compass._data.orientation_deg ;
  heading = H.maps(heading,0,360,360,0) - 90;
  return heading =  (heading<0)?heading + 360:heading;
}

void loop(){
  H.get_latlon(latitude,longitude);
  hmc5883l_read(&compass);
  heading = Compass(heading);
  string dat = to_string(latitude) + "," + to_string(longitude) + "," to_string(heading);
  unsigned char* data = (unsigned char*)dat.c_str();
  cout<<data;
//  bytes = U.write(data,port);
}

int main(){
  setup();
  while (1) loop();
}
