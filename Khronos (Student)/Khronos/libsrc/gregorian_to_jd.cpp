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
	 * Convert Gregorian date to Julian Day Number (JDN).
	 * Algorithm from Fliegel and Van Flandern (1968).
	 */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day) {
		// Adjust for months before March (January and February)
		long long a = (14 - month) / 12;
		long long y = year + 4800 - a;
		long long m = month + 12 * a - 3;

		// Calculate JDN (integer part represents noon)
		long long jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;

		// Return JDN at noon (integer value - 0.5 gives midnight, + 0 gives noon)
		return static_cast<jd_t>(jdn) - 0.5;
	}

	/**
	 * Convert Gregorian date and time to Julian Day (JD).
	 */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day,
	                      hour_t hour, minute_t minute, second_t second) {
		// Get JD at midnight
		jd_t jd = gregorian_to_jd(year, month, day);

		// Add time of day as fractional part
		// Note: JD 0.0 is noon, so midnight is -0.5
		// We need to add the time offset from midnight
		jd += tod(hour, minute, second);

		return jd;
	}

} // end-of-namespace khronos
