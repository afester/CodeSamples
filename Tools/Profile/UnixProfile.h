#ifndef __UNIX_PROFILE_H__
#define __UNIX_PROFILE_H__


#include <iostream>
#include <ostream>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>


using namespace std;

class TickCounter {
	typedef clock_t interval_type;
	interval_type m_start;
	interval_type m_end;
public:
	inline void start() {
           struct tms dummy;
           m_start = times(&dummy);
	}
	 
	inline void stop() {
           struct tms dummy;
           m_end = times(&dummy);
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
    struct rusage startValue;
    struct rusage endValue;

public:
	inline void start() {
            getrusage(RUSAGE_SELF, &startValue);
	}

	inline void stop() {
            getrusage(RUSAGE_SELF, &endValue);
	}

    /**
     * @return The number of microseconds the process spent executing kernel code
     */
	unsigned int getKernelTime() {
            struct timeval result;
            timersub(&endValue.ru_stime, &startValue.ru_stime, &result);
	    return result.tv_sec * 1000000 + result.tv_usec;
	}

    /**
     * @return The number of microseconds the process spent executing user code
     */
	unsigned int getUserTime() {
            struct timeval result;
            timersub(&endValue.ru_utime, &startValue.ru_utime, &result);
	    return result.tv_sec * 1000000 + result.tv_usec;
	}
};

ostream& operator<<(ostream& o, ProcessTimeCounter tc);

/*
class HiresCounter {
};
*/

class RealTimeCounter {
     struct timeval realStart;
     struct timeval realEnd;
public:
	inline void start() {
        gettimeofday(&realStart, NULL);
	}

	inline void stop() {
        gettimeofday(&realEnd, NULL);
	}

	unsigned int getRealTime() {
            struct timeval result;
            timersub(&realEnd, &realStart, &result);
	    return result.tv_sec * 1000000 + result.tv_usec;
	}
};

ostream& operator<<(ostream& o, RealTimeCounter tc);


#endif
