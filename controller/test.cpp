#include "udp/udp.h"
#include "helper/helper.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

Udp gs(3301);
Helper H;
vector<vector<double>> waypoints(5);
//use stod
void parse(unsigned char* str) {
    string data = H.toString(str);
    data = data.substr(1, data.size() - 2);
    vector<string> tokens = H.split(data, '!');
    for (vector<string>::iterator it = tokens.begin() ; it != tokens.end(); ++it)
        cout << ' ' << *it;
    cout << '\n';
    // cout << data << endl;
}
int main() {
    unsigned char *data;
    while(1) {
        data = gs.read();
        if (data[0] == '#')
            parse(data);
        // cout << data << endl;
    }
    return 0;
}
