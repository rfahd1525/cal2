/*
khronos\gregorian_to_jdn.cpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'gregorian::to_jdn' implementation.
*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	/**
	 * Convert a Gregorian date to Julian Day Number.
	 * Uses the algorithm for converting proleptic Gregorian calendar dates to JDN.
	 * @param year Astronomical year (1 CE = 1, 1 BCE = 0, 2 BCE = -1, etc.)
	 * @param month Month number [1..12]
	 * @param day Day of month [1..31]
	 * @return Julian Day Number at noon of the given date
	 */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day) {
		// Adjust the month and year for the algorithm
		long long a = (14 - month) / 12;
		long long y = year + 4800 - a;
		long long m = month + 12 * a - 3;

		// Calculate Julian Day Number (at noon, hence the .5)
		long long jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

		return static_cast<jd_t>(jdn) - 0.5;  // JD 0 is at noon, so subtract 0.5 for midnight
	}

	/**
	 * Convert a Gregorian date and time to Julian Day.
	 * @param year Astronomical year
	 * @param month Month number [1..12]
	 * @param day Day of month [1..31]
	 * @param hour Hour [0..23]
	 * @param minute Minute [0..59]
	 * @param second Second [0..60)
	 * @return Julian Day with fractional part for time
	 */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second) {
		jd_t jd = gregorian_to_jd(year, month, day);

		// Add the time of day as a fraction
		// JD noon is at .0, midnight is at .5
		jd += tod(hour, minute, second);

		return jd;
	}

} // end-of-namespace khronos
