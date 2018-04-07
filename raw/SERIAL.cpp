#include<iostream>
#include<fstream>
#include<string>
#include "SERIAL.h"

using namespace std;

SERIAL::SERIAL(string port){
  PORT = port;
  val = "";
  f.open(PORT);
}

string SERIAL::read(){
  f >> val;
}

string SERIAL::write(string value){
  f << value;
}

~SERIAL::SERIAL();
