#include<helper/helper.h>
#include<udp/udp.h>
#include <iostream>

using namespace std;

#define port 23907

Helper H;
Udp U(port);

int bytes;

double latitude,longitude;

void loop(){
  H.get_latlon(latitude,longitude);
  bytes = U.write();
}

int main(){
  while (1) loop();
}
