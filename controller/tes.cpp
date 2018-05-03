#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

int main(){
    string s = "12.123456";
    long long sum;
    int j = 0;
    for (int i = 0; i<s.length(); i++) {
        if(s[i] == '.') {
//          j++;
          continue;
        }
        sum += (s[i]-'0');
        sum *= 10;
    }
//    cout<<sum<<endl;
    double sum1 = (double)sum/10000000;
    printf("sum %.6f\n",sum1);
    printf("val %.6f",stod(s));
}

