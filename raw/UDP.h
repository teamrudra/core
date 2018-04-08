#ifndef UDP
#define UDP_H


class UDP
{
//  private: int socket;
  private: int PORT;
  private: int Bind;
  public:  UDP(int port);
  public:  int create_socket();
  public:  int create_socket(unsigned char* IP_ADDR, struct sockaddr_in client);
  public:  void read(unsigned char* buff, int length);
  public:  void send(unsigned char* buff, int length);
  public:  ~UDP();
};

#endif
