#ifndef __WIN32_PROFILE_H__
#define __WIN32_PROFILE_H__

#ifdef __cplusplus

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

ostream& operator<<(ostream& o, TickCounter tc);


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


ostream& operator<<(ostream& o, ProcessTimeCounter pt);

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

ostream& operator<<(ostream& o, RealTimeCounter rt);

#else

#include <time.h>
#include <stdio.h>
#include <unistd.h>

#include <windows.h>

typedef struct _ProcessTimeCounter {
   FILETIME kernelStart;
   FILETIME kernelEnd;
   FILETIME userStart;
   FILETIME userEnd;
}ProcessTimeCounter;

void ProcessTimeCounter_start(ProcessTimeCounter* obj) {
   FILETIME discard1;
   FILETIME discard2;
   GetProcessTimes(GetCurrentProcess(), &discard1, &discard2, &obj->kernelStart, &obj->userStart);
}

void ProcessTimeCounter_stop(ProcessTimeCounter* obj) {
   FILETIME discard1;
   FILETIME discard2;
   GetProcessTimes(GetCurrentProcess(), &discard1, &discard2, &obj->kernelEnd, &obj->userEnd);
}

unsigned int ProcessTimeCounter_getKernelTime(ProcessTimeCounter* obj) {
   ULARGE_INTEGER kernelStartCalc;
   ULARGE_INTEGER kernelEndCalc;

   kernelStartCalc.LowPart = obj->kernelStart.dwLowDateTime;
   kernelStartCalc.HighPart = obj->kernelStart.dwHighDateTime;
   kernelEndCalc.LowPart = obj->kernelEnd.dwLowDateTime;
   kernelEndCalc.HighPart = obj->kernelEnd.dwHighDateTime;

   return (kernelEndCalc.QuadPart - kernelStartCalc.QuadPart);
}

unsigned int ProcessTimeCounter_getUserTime(ProcessTimeCounter* obj) {
   ULARGE_INTEGER userStartCalc;
   ULARGE_INTEGER userEndCalc;

   userStartCalc.LowPart = obj->userStart.dwLowDateTime;
   userStartCalc.HighPart = obj->userStart.dwHighDateTime;
   userEndCalc.LowPart = obj->userEnd.dwLowDateTime;
   userEndCalc.HighPart = obj->userEnd.dwHighDateTime;

   return (userEndCalc.QuadPart - userStartCalc.QuadPart) / 10;
}

const char* ProcessTimeCounter_toString(ProcessTimeCounter* obj) {
   static const char* format = "CPU Time: kernel=%dus (%dms), user=%dus (%dms)";
   unsigned int kernelTime = ProcessTimeCounter_getKernelTime(obj);
   unsigned int userTime = ProcessTimeCounter_getUserTime(obj);

   char checkSize = 0;
   int size = snprintf(&checkSize, sizeof(checkSize), format, kernelTime, kernelTime/1000, userTime, userTime/1000) + 1;

   char* result = calloc(size, sizeof(char));
   snprintf(result, size, format, kernelTime, kernelTime/1000, userTime, userTime/1000);
   return result;
}

#endif

#endif
