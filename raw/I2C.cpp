#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <iostream>
#include "I2C.h"

using namespace std;

I2C::I2C(int address){
  I2C_ADDR = address;
  if ((I2C_fd = open("/dev/i2c-1", O_RDWR)) < 0){
      //Open port for reading and writing
      cerr<<"Failed to open i2c bus\n";
  }
  if (ioctl(I2C_fd, I2C_SLAVE, I2C_ADDR) < 0) {
     cerr<<"Device not present"<<endl;
  }
}
void I2C::writeToDevice(unsigned char *data, int length){
  if (write(I2c_fd, data, length) != length)  {
     cerr<<"Can't write to Device\n";
  }
}

void I2C::readfromtDevice(unsigned char* data,int length){
  if (read(I2C_fd, data, length) != length) {
    cout<<"Unable to read from device\n";
  }
}

~I2C::I2C(){}
