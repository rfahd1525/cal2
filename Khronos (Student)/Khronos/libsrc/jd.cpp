/**	@file Jdn.cpp
	@author Garth Santor
	@date 2012-09-16

	Date and time library to_string method implementations.
	*/


#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <ctime>

namespace khronos {

	// Default constructor - initializes to now with time
	Jd::Jd() {
		time_t now = time(nullptr);
		struct tm local_time;
		localtime_s(&local_time, &now);

		jd_ = gregorian_to_jd(
			year_t(local_time.tm_year + 1900),
			month_t(local_time.tm_mon + 1),
			day_t(local_time.tm_mday),
			hour_t(local_time.tm_hour),
			minute_t(local_time.tm_min),
			second_t(local_time.tm_sec)
		);
	}

	// Constructor with now_t parameter
	Jd::Jd(now_t now) {
		time_t current_time = time(nullptr);
		struct tm local_time;
		localtime_s(&local_time, &current_time);

		if (now == NOTIMEOFDAY) {
			// Just the date, no time (noon)
			jd_ = gregorian_to_jd(
				year_t(local_time.tm_year + 1900),
				month_t(local_time.tm_mon + 1),
				day_t(local_time.tm_mday)
			);
		}
		else {
			// With time
			jd_ = gregorian_to_jd(
				year_t(local_time.tm_year + 1900),
				month_t(local_time.tm_mon + 1),
				day_t(local_time.tm_mday),
				hour_t(local_time.tm_hour),
				minute_t(local_time.tm_min),
				second_t(local_time.tm_sec)
			);
		}
	}

} // end-of-namespace khronos
