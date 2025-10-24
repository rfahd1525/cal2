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
	 * Convert Julian calendar date to Julian Day Number.
	 * Uses Fliegel and Van Flandern algorithm adapted for Julian calendar.
	 */
	jd_t julian_to_jd(year_t year, month_t month, day_t day) {
		long long a = (14 - month) / 12;
		long long y = year + 4800 - a;
		long long m = month + 12 * a - 3;

		long long jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - 32083;

		return static_cast<jd_t>(jdn) - 0.5;
	}

	/**
	 * Convert Julian calendar date and time to Julian Day.
	 */
	jd_t julian_to_jd(year_t year, month_t month, day_t day,
	                   hour_t hour, minute_t minute, second_t second) {
		jd_t jd = julian_to_jd(year, month, day);
		jd += tod(hour, minute, second);
		return jd;
	}

	/**
	 * Convert Julian Day to Julian calendar date.
	 */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day) {
		long long jdn = static_cast<long long>(std::floor(jd + 0.5));

		long long a = jdn + 32082;
		long long b = (4 * a + 3) / 1461;
		long long c = a - (1461 * b) / 4;
		long long d = (5 * c + 2) / 153;

		day = static_cast<day_t>(c - (153 * d + 2) / 5 + 1);
		month = static_cast<month_t>(d + 3 - 12 * (d / 10));
		year = static_cast<year_t>(b - 4800 + d / 10);
	}

	/**
	 * Convert Julian Day to Julian calendar date and time.
	 */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day,
	                   hour_t& hour, minute_t& minute, second_t& second) {
		jd_to_julian(jd, year, month, day);

		double jdn = std::floor(jd + 0.5);
		tod_t timeOfDay = jd - (jdn - 0.5);

		jd_to_hms(timeOfDay, hour, minute, second);
	}

} // end-of-namespace khronos
