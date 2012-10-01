
#include <string>
#include <ctime>
#include <iostream>

#include "aw_common.h"

namespace aw {
    
// example here:
// http://felix.abecassis.me/2011/09/cpp-timer-raii/
class Timer {
public:
    explicit Timer(const std::string& n);
    void start();
    void end(); 
	friend std::ostream& operator<<(std::ostream& output, const Timer& t);
    
    
private:
	//! Since this function is called by operator<<, it needs to be const, otherwise we cannot pass the const Timer inst to this function. 
	double _get_ms_difference(double start, double end) const;

	bool _stopped;
    std::string _name;
	// data format is clock_t
    std::clock_t _start_time;
    std::clock_t _end_time;
	
};
    
    
}