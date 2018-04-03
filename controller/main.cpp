#include <iostream>
#include "udp/udp.h"

using namespace std;

int main() {
    Udp groundStation(23907);
    unsigned char *output;
    while (1) {
        output = (unsigned char*)groundStation.read();
        for (int i=0;output[i]!='\0';i++)
            cout << output[i];
        cout << endl;
        cout << groundStation.write(output);
    }
    return 0;
}
