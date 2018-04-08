#include "udp/udp.h"
#include <iostream>


using namespace std;

int main() {
    Udp groundStation(23907);
    unsigned char *output;
    while (1) {
        output = (unsigned char*)groundStation.read();
        cout << output << endl;
        cout << groundStation.write((unsigned char*)"Hello Bro!");
    }
    return 0;
}
