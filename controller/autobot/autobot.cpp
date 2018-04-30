#include "autobot.h"
#include "../helper/helper.h"

Autobot::Autobot(){
}
int Autobot::update(double heading, Helper H){
  H.get_latlon(latitude, longitude);
  bearing = H.get_bearing(latitude,longitude,destlat,destlon);
  distance = H.get_dist(latitude,longitude,destlat,destlon);
  difference = heading - bearing;
  difference = (abs(difference)>180)?-((360-abs(difference))*difference/abs(difference)):difference;

//  printf("latitude %.6f longitude %.6f ",latitude,longitude);
//  cout<<"heading: "<<heading<<" bearing: "<<bearing<<" difference: "<<difference<<endl;

  if( distance < ACCURACY){
    dat = -1;
  }
  else {
    if(difference > err) {
      if(dat == stop || dat == Left ) dat = Left;
      else if(dat == fwd ||dat == Leftdrift) dat = Leftdrift;
      else dat = stop;
    }else if(difference <- err){
      if(dat == stop || dat == Right ) dat = Right;
      else if(dat == fwd ||dat == Rightdrift) dat = Rightdrift;
      else dat = stop;
    }
    else{
      if(dat==Leftdrift || dat == Rightdrift || dat == fwd || dat == stop) dat = fwd;
      else dat = stop;
    }
  }
  return dat;
}
