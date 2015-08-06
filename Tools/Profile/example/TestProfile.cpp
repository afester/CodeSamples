#include <iostream>
#include <unistd.h>

using namespace std;

#include "Profile.h"

void f(int x) {
}

int main() {
	TickCounter c1;
	ProcessTimeCounter p1;
	RealTimeCounter r1;

	c1.start();
	p1.start();
    r1.start();

	//for (int i = 0;  i < 1650;  i++) 
	for (int i = 0;  i < 880;  i++) 
        for (int j = 0;  j < 100000;  j++) { int x = i + j; f(x); }
	usleep(500000);

	c1.stop();
	p1.stop();
    r1.stop();

	cerr << c1 << endl;
	cerr << p1 << endl;
	cerr << r1 << endl;
}
