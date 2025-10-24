/*
khronos\jd_to_julian.cpp
(c) Garth Santor
Created: 2015-09-22

Khronos library 'julian::from_jd' implementation.
*/

#include <khronos/julian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	/**
	 * Convert Julian Day Number to Julian calendar date.
	 * @param jd Julian Day Number
	 * @param year Output: astronomical year
	 * @param month Output: month [1..12]
	 * @param day Output: day of month [1..31]
	 */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day) {
		long long jdn = static_cast<long long>(std::floor(jd + 0.5));

		long long a = jdn + 32082;
		long long b = (4 * a + 3) / 1461;
		long long c = a - (1461 * b) / 4;
		long long d = (4 * c + 3) / 1461;
		long long e = c - (1461 * d) / 4;
		long long m = (5 * e + 2) / 153;

		day = static_cast<day_t>(e - (153 * m + 2) / 5 + 1);
		month = static_cast<month_t>(m + 3 - 12 * (m / 10));
		year = static_cast<year_t>(100 * b + d - 4800 + m / 10);
	}

	/**
	 * Convert Julian Day to Julian calendar date and time.
	 * @param jd Julian Day with fractional part
	 * @param year Output: astronomical year
	 * @param month Output: month [1..12]
	 * @param day Output: day of month [1..31]
	 * @param hour Output: hour [0..23]
	 * @param minute Output: minute [0..59]
	 * @param second Output: second [0..60)
	 */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second) {
		jd_to_julian(jd, year, month, day);

		double jd_floor = std::floor(jd + 0.5);
		double tod_fraction = jd - (jd_floor - 0.5);

		if (tod_fraction < 0) tod_fraction += 1.0;
		if (tod_fraction >= 1.0) tod_fraction -= 1.0;

		jd_to_hms(tod_fraction, hour, minute, second);
	}

} // end-of-namespace khronos
