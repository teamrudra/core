#ifndef SERIAL
#define SERIAL_H


class SERIAL
{
  private: string PORT;
  private: fstream f;
  public:  string val;
  public: SERIAL(string port);
  public: void read();
  public: void write(string value);
  public: ~SERIAL();
};

#endif
