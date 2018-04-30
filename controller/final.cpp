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
#define portW 23907
#define portR 3301

Spi drive(DRIVE,speed);
Spi arm(ARM, speed);
HMC5883L compass;
Udp gs(portR);
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

string message = "";

unsigned char* value;

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
   kill();
}

double Compass() {
  hmc5883l_read(&compass);
  heading = compass._data.orientation_deg ;
  heading = H.maps(heading,0,360,360,0) - 90;
  heading = (heading<0)?heading + 360:heading;
  message = "!" + to_string(heading) + "!";
  value = (unsigned char*)message.c_str();
  gs.write(value,portW);
}

void removeCoods(int check){
  if(check){
    for(int i = 1;i < size;i++){
      cord[i-1].destlat = cord[i].destlat;
      cord[i-1].destlon = cord[i].destlon;
    }
   size--;
 }else{
   for(int i=0;i<size;i++){
     cord[i].destlat = 0;
     cord[i].destlon = 0;
   }
   size = 0;
 }
}

void autonomous() {
 while(1){
    coods = gs.read(0,200000);
    if(coods[0] == '$') {
      removeCoods(0);
      break;
    }
    Compass();
    A.destlat = cord[0].destlat,A.destlon = cord[0].destlon;
  //  A.destlat = 12.821186, A.destlon = 80.038238;
    dat = A.update(heading,H);
    if(dat == -1){
      break;
    }
    else {
      data = (unsigned char)dat;
      drive.RW(&data,1);
    }
  }
}

void getdestlatlon(string point,int i){
  vector<string> token = H.split(point, ',');
  cord[i].destlat = stod(token[0]);
  cord[i].destlon = stod(token[1]);
  cout<<cord[i].destlat<<" "<<cord[i].destlon<<endl;
}

int parseCoods(unsigned char* coods) {
  string data = H.toString(coods);
  data = data.substr(1, data.size() - 2);
  vector<string> tokens = H.split(data, '!');
  int i = 0;
  for (vector<string>::iterator it = tokens.begin() ; it != tokens.end(); ++it,i++){
    getdestlatlon(*it,i);
  }
  return tokens.size();
}

void keyboard(unsigned char *coods) {
  static int i, mid;
  Compass();
  static unsigned char controls[2];
  for (i = 0; coods[i]!='>'; i++)
    mid = (coods[i] == ',') ? i : mid;
  controls[0] = H.parse(coods, 1, mid);
  controls[1] = H.parse(coods, mid+1, i);
  drive.RW(&controls[0],1);
  arm.RW(&controls[1], 1);
}

void check(unsigned char* coods) {
    isAuto = (coods[0] == '$')? 2 : isAuto;
    isAuto = (coods[0] == '#')? 1 : isAuto;
    isAuto = (coods[0] == '<')? 0 : isAuto;
    if (isAuto == 2 && size > 0 ) {
          removeCoods(0);
          cout << "Autonomous Stopped" << endl;
    }
    else if(isAuto == 1){
          size = parseCoods(coods);
          cout << "Autonomous running" << endl;
          cout<<heading;
          message = "$" + to_string(heading) + ",%";
          value = (unsigned char*)message.c_str();
          gs.write(value,portW);
          if(coods[0] == '@'){
            autonomous();
            message = "$" + to_string(heading) + ",Final destination: " + to_string(cord[0].destlat) + " " + to_string(cord[0].destlon) + "reached,";
            value = (unsigned char*)message.c_str();
            gs.write(value,portW);
            removeCoods(1);
          }
          else if(coods[0] == '*'){
            for(int i=0;i<size;i++){
              autonomous();
              if(i == size-1)
                message = "$" + to_string(heading) + ",Final destination: " + to_string(cord[0].destlat) + " " + to_string(cord[0].destlon) + "reached,";
              else
                message = "$" + to_string(heading) + ",Destination: " + to_string(cord[0].destlat) + " " + to_string(cord[0].destlon) + "reached,";

              value = (unsigned char*)message.c_str();
              gs.write(value,portW);
              removeCoods(1);
            }
          }
    }
    else {
          cout << "Keyboard running" << endl;
          keyboard(coods);
    }
}

void loop() {
  coods = gs.read(1,0);
  cout<<" "<<coods[0]<<endl;
  if(coods[0] != '0') {
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
