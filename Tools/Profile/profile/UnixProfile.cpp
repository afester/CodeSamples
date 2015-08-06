
#include "UnixProfile.h"

ostream& operator<<(ostream& o, TickCounter tc) {
	o << "Ticks: " << tc.get_period_count();
	return o;
}


ostream& operator<<(ostream& o, ProcessTimeCounter pt) {
	o << "CPU Time: kernel=" << pt.getKernelTime() << "us (" << pt.getKernelTime()/1000 << "ms), user=" 
                             << pt.getUserTime() << "us (" << pt.getUserTime()/1000 << "ms)";
	return o;
}


ostream& operator<<(ostream& o, RealTimeCounter rt) {
	o << "Real Time: " << rt.getRealTime() << "us (" << rt.getRealTime()/1000 << "ms)";
	return o;
}
