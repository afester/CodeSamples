#include<iostream>

using namespace std;

void outputSample() {
    int i;
    char c;
    for(i = 40; i <= 126; i++) {
        c = i;
        cout<<dec<<i<<"-"<<hex<<i<<"-"<<oct<<i<<"-"<<c<<"\n";
    }
}
