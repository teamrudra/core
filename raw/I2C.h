#ifndef I2C
#define I2C_H


class I2C
{
  private: int I2C_ADDR;
  private: int I2C_fd;
  private: unsigned char buf[1024];
  public:  I2C(int address);
  public:  void writeToDevice(unsigned char *data, int length);
  public:  void readfromtDevice(unsigned char *data,int length);
  public:  ~I2C();
};

#endif
