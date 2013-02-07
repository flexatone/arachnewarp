#include <ctime>
#include <string>
#include <iostream>

#include "aw_timer.h"

namespace aw {
    
Timer :: Timer(const std::string& n) 
    : _name(n), _start_time(0), _end_time(0), _stopped(false) {
}

void Timer :: start() {
    _start_time = std::clock();
    _stopped = false;    
}
    
void Timer :: stop() {
    _end_time = std::clock();
    _stopped = true;
}

//double Timer :: _get_ms_difference(double start, double end) const {
//    // this will cast inputs given as std::clock_t values
//    return ((end-start) / double(CLOCKS_PER_SEC)) * 1000.0;
//}


double Timer :: _get_ms_difference(std::clock_t start, std::clock_t end) const {
    // difftime (time2-time1), returns floating point double of time in sec.
    // just convert to msec
    return std::difftime(end, start) / 1000.0;
}



std::ostream& operator<<(std::ostream& ostream, const Timer& t) {
    // get time difference but do not stop
    double dif(0);
    if (not (t._stopped)) { // if not stopped, get comparison to now
        dif = t._get_ms_difference(t._start_time, std::clock());
    }
    else {
        dif = t._get_ms_difference(t._start_time, t._end_time);        
    }
    ostream << "<Timer: " << t._name << ": " << dif << " msec>";
    return ostream; 
}

    
    
}