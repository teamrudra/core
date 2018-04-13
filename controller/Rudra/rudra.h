#ifndef __RUDRA_H__
#define __RUDRA_H__

#include <libgpsmm.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <cmath>
using namespace std;

#define d2r 0.0174532914
#define r2d 57.2957763671

class Rudra {
    private:
    public:
      Rudra();
      int gpsdintialise();
      void get_latlon(double &latitude,double &longitude);
      double maps(double x, double in_min, double in_max, double out_min, double out_max);
      float get_bearing(float lat1, float lon1, float lat2, float lon2);
      float get_dist(float lat1, float lon1, float lat2, float lon2);
      unsigned char parse(unsigned char *n, int start, int end) ;

};

#endif

