#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> //for handeling file discriptors

using namespace std;

#define buflen 14
#define port 3301 //The port in which the connection has to be made to connect with the server.
#define ip "192.168.43.216" //Host address has to be entered here.

int main(){
        sockaddr_in si_me, si_other;
        int s, i, slen=sizeof(si_other),recv_len;
        char buf[buflen];
        for(int i=0;i<buflen;i++)
                buf[i] = ' ';
        s=socket(AF_INET, SOCK_DGRAM, 0); // Creating a socket with IPv4 protocal (AF_INET) for UDP (SOCK_DGRAM) communication.
        if(s<0) {
                cout<<"Socket failed"<<endl;
                return 0;
        }
        memset((char *) &si_other, 0, sizeof(si_other)); //Initializing all the values of the structure to 0.
        si_me.sin_family=AF_INET; //IPv4
        si_me.sin_port=htons(port);  //host to network short
        si_me.sin_addr.s_addr=INADDR_ANY; //Uses any address that is available.
        if(bind(s, (struct sockaddr*)&si_me     , sizeof(si_me))<0) //Binding to the socket using the file discriptor
        {
                cout<<"Binding failed"<<endl;
                return 0;
        }
        cout<<"listening.."<<endl;
        while(1){
                recv_len=recvfrom(s,buf,buflen,0,(struct sockaddr*)&si_other,(socklen_t *)&slen);
                cout<<recv_len<<endl;
                if(recv_len<0)
			cout<<"ERROR"<<endl;
		else{
			cout<<buf<<endl;
			continue;
		}
        }
        close(s);
        return 0;
}

