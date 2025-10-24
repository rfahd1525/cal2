/*
khronos\jdn_to_gregorian.cpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'gregorian::from_jdn' implementation.
*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	/**
	 * Convert Julian Day Number to Gregorian date.
	 * Algorithm from Fliegel and Van Flandern (1968).
	 */
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day) {
		// Add 0.5 to get the JDN (Julian Day Number at noon)
		long long jdn = static_cast<long long>(std::floor(jd + 0.5));

		long long a = jdn + 32044;
		long long b = (4 * a + 3) / 146097;
		long long c = a - (146097 * b) / 4;
		long long d = (4 * c + 3) / 1461;
		long long e = c - (1461 * d) / 4;
		long long m = (5 * e + 2) / 153;

		day = static_cast<day_t>(e - (153 * m + 2) / 5 + 1);
		month = static_cast<month_t>(m + 3 - 12 * (m / 10));
		year = static_cast<year_t>(100 * b + d - 4800 + m / 10);
	}

	/**
	 * Convert Julian Day to Gregorian date and time.
	 */
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day,
	                      hour_t& hour, minute_t& minute, second_t& second) {
		// Get the date part
		jd_to_gregorian(jd, year, month, day);

		// Get the time part (fractional part of JD)
		// JD has fractional day where 0.0 = noon, so we need to extract time from midnight
		double jdn = std::floor(jd + 0.5);
		tod_t timeOfDay = jd - (jdn - 0.5);  // time from midnight

		jd_to_hms(timeOfDay, hour, minute, second);
	}

} // end-of-namespace khronos
