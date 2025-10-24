/**	@file Julian_to_jd.cpp
	@author Garth Santor
	@date 2012-09-14

	Date and time library Julian conversion implementations.
	*/

#include <khronos/julian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	/**
	 * Convert a Julian calendar date to Julian Day Number.
	 * @param year Astronomical year
	 * @param month Month number [1..12]
	 * @param day Day of month [1..31]
	 * @return Julian Day Number at noon of the given date
	 */
	jd_t julian_to_jd(year_t year, month_t month, day_t day) {
		// Adjust for algorithm
		long long a = (14 - month) / 12;
		long long y = year + 4800 - a;
		long long m = month + 12 * a - 3;

		// Julian calendar formula
		long long jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083;

		return static_cast<jd_t>(jdn) - 0.5;
	}

	/**
	 * Convert a Julian calendar date and time to Julian Day.
	 * @param year Astronomical year
	 * @param month Month number [1..12]
	 * @param day Day of month [1..31]
	 * @param hour Hour [0..23]
	 * @param minute Minute [0..59]
	 * @param second Second [0..60)
	 * @return Julian Day with fractional part for time
	 */
	jd_t julian_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second) {
		jd_t jd = julian_to_jd(year, month, day);
		jd += tod(hour, minute, second);
		return jd;
	}

} // end-of-namespace khronos
