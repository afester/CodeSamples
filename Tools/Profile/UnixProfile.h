#ifndef __UNIX_PROFILE_H__
#define __UNIX_PROFILE_H__


#include <iostream>
#include <ostream>

using namespace std;

#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include <windows.h>

class TickCounter {
	typedef DWORD interval_type;
	interval_type m_start;
	interval_type m_end;

public:
	inline void start() {
		m_start = ::GetTickCount();
	}
	 
	inline void stop() {
		m_end = ::GetTickCount();
	}

	inline interval_type get_period_count() const {
		return static_cast<interval_type>(m_end - m_start);
	}

	inline interval_type get_seconds() const {
		return get_period_count() / interval_type(1000);
	}

	inline interval_type get_milliseconds() const {
		return get_period_count();
	}

	inline interval_type get_microseconds() const {
		return get_period_count() * interval_type(1000);
	}
};

ostream& operator<<(ostream& o, TickCounter tc) {
	o << "Ticks: " << tc.get_period_count();
	return o;
}



class ProcessTimeCounter {
	FILETIME kernelStart;
	FILETIME kernelEnd;
	FILETIME userStart;
	FILETIME userEnd;

public:
	inline void start() {
		FILETIME discard1;
		FILETIME discard2;
		::GetProcessTimes(::GetCurrentProcess(), &discard1, &discard2, &kernelStart, &userStart);
	}

	inline void stop() {
		FILETIME discard1;
		FILETIME discard2;
		::GetProcessTimes(::GetCurrentProcess(), &discard1, &discard2, &kernelEnd, &userEnd);
	}

    /**
     * @return The number of microseconds the process spent executing kernel code
     */
	unsigned int getKernelTime() {
		ULARGE_INTEGER kernelStartCalc;
		kernelStartCalc.LowPart = kernelStart.dwLowDateTime;
		kernelStartCalc.HighPart = kernelStart.dwHighDateTime;
		ULARGE_INTEGER kernelEndCalc;
		kernelEndCalc.LowPart = kernelEnd.dwLowDateTime;
		kernelEndCalc.HighPart = kernelEnd.dwHighDateTime;

		return (kernelEndCalc.QuadPart - kernelStartCalc.QuadPart);
	}

    /**
     * @return The number of microseconds the process spent executing user code
     */
	unsigned int getUserTime() {
		ULARGE_INTEGER userStartCalc;
		userStartCalc.LowPart = userStart.dwLowDateTime;
		userStartCalc.HighPart = userStart.dwHighDateTime;
		ULARGE_INTEGER userEndCalc;
		userEndCalc.LowPart = userEnd.dwLowDateTime;
		userEndCalc.HighPart = userEnd.dwHighDateTime;

		return (userEndCalc.QuadPart - userStartCalc.QuadPart) / 10;
	}
};


ostream& operator<<(ostream& o, ProcessTimeCounter pt) {
	o << "CPU Time: kernel=" << pt.getKernelTime() << "us (" << pt.getKernelTime()/1000 << "ms), user=" 
                             << pt.getUserTime() << "us (" << pt.getUserTime()/1000 << "ms)";
	return o;
}

/*
class HiresCounter {
};
*/

class RealTimeCounter {
	FILETIME realStart;
	FILETIME realEnd;

public:
	inline void start() {
        GetSystemTimeAsFileTime(&realStart);
	}

	inline void stop() {
        GetSystemTimeAsFileTime(&realEnd);
	}

	unsigned int getRealTime() {
		ULARGE_INTEGER realStartCalc;
		realStartCalc.LowPart = realStart.dwLowDateTime;
		realStartCalc.HighPart = realStart.dwHighDateTime;
		ULARGE_INTEGER realEndCalc;
		realEndCalc.LowPart = realEnd.dwLowDateTime;
		realEndCalc.HighPart = realEnd.dwHighDateTime;

		return (realEndCalc.QuadPart - realStartCalc.QuadPart) / 10;
	}
};

ostream& operator<<(ostream& o, RealTimeCounter rt) {
	o << "Real Time: " << rt.getRealTime() << "us (" << rt.getRealTime()/1000 << "ms)";
	return o;
}

#endif
