#include "helper/helper.h"
#include "hmc5883l/hmc5883l.h"
#include "udp/udp.h"
#include "spi/spi.h"
#include "autobot/autobot.h"
#include <wiringSerial.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#define DRIVE 1
#define ARM 0
#define speed 1000000
#define port 3301

Spi drive(DRIVE,speed);
Spi arm(ARM, speed);
HMC5883L compass;
Udp gs(port);
Helper H;
Autobot A;

int dat,isAuto,size;
unsigned char data;
unsigned char *coods;

//double destlat = 12.821186, destlon = 80.038238;
double heading;

struct waypoints{
  double destlat;
  double destlon;
};

struct waypoints cord[5];

void kill() {
  cout<<"Killed";
  static unsigned char controls[2] = { (char)0, (char)0 };
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}


void setup() {
    dat = 0,isAuto = 0,size = 0;
//    coods = NULL;
   if(!H.gpsdintialise())
    cout<<"ERROR: initialising gpsd! "<<endl;
    if( hmc5883l_init(&compass) != HMC5883L_OKAY )
      cout << "ERROR: initialising compass!" << endl;

//    kill();
}

double Compass(double heading) {
  heading = compass._data.orientation_deg ;
  heading = H.maps(heading,0,360,360,0) - 90;
  heading = (heading<0)?heading + 360:heading;
  unsigned char* head = (unsigned char*)to_string(heading).c_str();
  gs.write(head,port);
  return heading;
}

void removeCoods(){
  for(int i = 1;i < size;i++){
    cord[i-1].destlat = cord[i].destlat;
    cord[i-1].destlon = cord[i].destlon;
  }
  size--;
}


void autonomous() {
  hmc5883l_read(&compass);
  heading = Compass(heading);
  A.destlat = cord[0].destlat,A.destlon = cord[0].destlon;
//  A.destlat = 12.821186, A.destlon = 80.038238;
  A.update(heading,H);
  if(dat == -1){
    gs.write((unsigned char*)('*'),3301);
    removeCoods();
    //delay()
  }
  else {
    data = (unsigned char)dat;
    drive.RW(&data,1);
  }
}

void getdestlatlon(string point,int i){
  vector<string> token = H.split(point, ',');
  cord[i].destlat = stod(token[0]);
  cord[i].destlon = stod(token[1]);
}

int parseCoods(unsigned char* coods) {
  string data = H.toString(coods);
  data = data.substr(1, data.size() - 2);
  vector<string> tokens = H.split(data, '!');
  int i = 0;
  for (vector<string>::iterator it = tokens.begin() ; it != tokens.end(); ++it,i++){
    getdestlatlon(*it,i);
//    cout << ' ' << *it;
  }
  return tokens.size();
}

void keyboard(unsigned char *coods) {
  static int i, mid;
  static unsigned char controls[2];
  for (i = 0; coods[i]!='>'; i++)
    mid = (coods[i] == ',') ? i : mid;
  controls[0] = H.parse(coods, 1, mid);
  controls[1] = H.parse(coods, mid+1, i);
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}

void check(unsigned char* coods) {
 if(coods != (unsigned char* )"0"){
  isAuto = (coods[0] == '$')? 2 : isAuto;
  isAuto = (coods[0] == '#')? 1 : isAuto;
  isAuto = (coods[0] == '<')? 0 : isAuto;
  if (isAuto == 2 && size > 0 ) {
    removeCoods();
    cout << "Autonomous Stopped" << endl;
  }
  else if(isAuto == 1){
    gs.write((unsigned char*)('*'),3301);
    size = parseCoods(coods);
    cout << "Autonomous running" << endl;
    autonomous();
  }
  else if(!isAuto){
    cout << "Keyboard running" << endl;
    keyboard(coods);
  }
}


void loop() {
  coods = gs.read();
  if(coods != (unsigned char* )"0" || isAuto == 1) {
     check(coods);
  }
  else {
     kill();
  }
}

int main(){
    setup();
    while (1) loop();
    return 0;
}
