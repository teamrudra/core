#include <iostream>
using namespace std;

int parse(char buf[]) {
    int a = 0;
    for (int i = 0 ;buf[i]!='\0';i++) {
        a += (buf[i]-'0');
        a *= 10;
    }
    return a/10;
}

bool getBit(int n, int pos) {
  return (n >> pos) & 1;
}

int main() {
    // char ch[] = "123";
    // cout << parse(ch) << endl;
    cout << getBit(2, 13) << endl;
    return 0;
}