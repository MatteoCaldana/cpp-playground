#include<chrono>

class Chrono{
private:
    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
public:
    void reset() { begin = std::chrono::steady_clock::now(); }
    auto split() { return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - begin).count(); }
};

class WallClockTimer {
public:
    struct timeval t1, t2;
    WallClockTimer() :
        t1(), t2() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    void reset() {
        gettimeofday(&t1, 0);
        t2 = t1;
    }
    unsigned long long elapsed() {
        return ((t2.tv_sec - t1.tv_sec) * 1000ULL * 1000ULL) + ((t2.tv_usec - t1. tv_usec));
    }
    unsigned long long split() {
        gettimeofday(&t2, 0);
        return elapsed();
    }
};

class CPUTimer {
public:
    //clock_t t1, t2;
    struct rusage t1,t2;

    CPUTimer() :
        t1(), t2() {
        getrusage(RUSAGE_SELF, &t1);
        //t1 = clock();
        t2 = t1;
    }
    void reset() {
        getrusage(RUSAGE_SELF, &t1);
        //t1 = clock();
        t2 = t1;
    }
    // proxy for userelapsed
    unsigned long long elapsed() {
        return totalelapsed();//userelapsed();
    }

    unsigned long long totalelapsed() {
        return userelapsed() + systemelapsed();
    }
    // returns the *user* CPU time in micro seconds (mu s)
    unsigned long long userelapsed() {
        return ((t2.ru_utime.tv_sec - t1.ru_utime.tv_sec) * 1000ULL * 1000ULL) + ((t2.ru_utime.tv_usec - t1.ru_utime.tv_usec)
                );
    }

    // returns the *system* CPU time in micro seconds (mu s)
    unsigned long long systemelapsed() {
        return ((t2.ru_stime.tv_sec - t1.ru_stime.tv_sec) * 1000ULL * 1000ULL) + ((t2.ru_stime.tv_usec - t1.ru_stime.tv_usec)
                );
    }

    unsigned long long split() {
        getrusage(RUSAGE_SELF, &t2);
        return elapsed();
    }
};