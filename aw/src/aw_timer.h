#ifndef _AW_TIMER_H_
#define _AW_TIMER_H_

#include <string>
#include <ctime>
#include <iostream>

#include "aw_common.h"

namespace aw {
    
// example here:
// http://felix.abecassis.me/2011/09/cpp-timer-raii/

//! A Timer class. 
class Timer {

    public://-------------------------------------------------------------------    

    explicit Timer(const std::string& n);
    void start();
    void stop(); 
	friend std::ostream& operator<<(std::ostream& output, const Timer& t);
    
    private://----------------------------------------------------------------
	//! Since this function is called by operator<<, it needs to be const, otherwise we cannot pass the const Timer inst to this function. 
	double _get_ms_difference(double start, double end) const;

	//! Storage for the name of the timer. 
    std::string _name;
		
	//! Data format std::clock_t is natively returned by std::clocl()
    std::clock_t _start_time;
    std::clock_t _end_time;
	bool _stopped;	

};
    
}

#endif // ends _AW_TIMER_H_


