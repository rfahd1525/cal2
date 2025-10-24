/**	@file Islamic_to_jd.cpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar conversion implementations.
	*/

#include <khronos/islamic_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	/**
	 * Convert Islamic calendar date to Julian Day Number.
	 * Based on the arithmetic Islamic calendar algorithm.
	 */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day) {
		// Calculate days from the epoch
		long long y = year - 1;
		long long cycles = y / 30;
		long long year_in_cycle = y % 30;

		// Days in complete 30-year cycles (each cycle has 10631 days)
		long long days = cycles * 10631;

		// Days in complete years within current cycle
		// Each year has 354 days, plus 1 day for leap years
		for (int i = 1; i <= year_in_cycle; ++i) {
			days += 354;
			if (is_islamic_leapyear(i)) {
				days++;
			}
		}

		// Add days from complete months in current year
		for (int m = 1; m < month; ++m) {
			days += islamic_days_in_month(m, is_islamic_leapyear(year));
		}

		// Add days in current month
		days += day;

		return ISLAMIC_EPOCH + days - 1;
	}

	/**
	 * Convert Islamic calendar date and time to Julian Day.
	 */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second) {
		jd_t jd = islamic_to_jd(year, month, day);
		jd += tod(hour, minute, second);
		return jd;
	}

} // end-of-namespace khronos
