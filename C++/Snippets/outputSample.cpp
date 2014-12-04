#include<iostream>
#include "Samples.h"

using namespace std;


OutputSample::OutputSample() : Sample("Output sample") {
}

void OutputSample::run() {
    int i;
    char c;
    for(i = 40; i <= 126; i++) {
        c = i;
        cout<<dec<<i<<"-"<<hex<<i<<"-"<<oct<<i<<"-"<<c<<"\n";
    }
}
