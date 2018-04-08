#include <iostream>
#include <wiringPiSPI.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstring>
#include <fcntl.h>
#include <arpa/inet.h>

using namespace std;

UDP::UDP(int port){
  PORT = port;
}

int UDP::create_socket(){
  int socket, bind;

  struct sockaddr_in addr;
  memset((char *)&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);

  if ((socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      return socket;
  if ((bind = bind(sock, (struct sockaddr *)&addr, sizeof(addr))) < 0)
      return bind;
  return socket;

}

int UDP::create_socket(unsigned char* IP_ADDR){
  int socket, bind;

  struct sockaddr_in clinet;
  memset((char *)&clinet, 0, sizeof(clinet));

  client.sin_family = AF_INET;
  client.sin_addr.s_addr = inet_addr(IP_ADDR);
  client.sin_port = htons(port);

  if ((socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
      return socket;
  if ((bind = bind(sock, (struct sockaddr *)&client, sizeof(client))) < 0)
      return bind;
  return socket;

}

void UDP::read(unsigned char* buff, int BUFSIZE, int fd, (struct sockaddr *)*remaddr, int *addrlen){
  recvlen = recvfrom(fd, buff, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
  if(recvlen<0)
      cout<<"ERROR"<<endl;
}

void UDP::send(unsigned char* buff, int BUFSIZE,int fd){
  send(fd, buff, BUFSIZE, 0);
}
