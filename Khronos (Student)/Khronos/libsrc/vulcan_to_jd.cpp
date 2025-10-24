/**	@file vulcan_to_jd.cpp
	@author Garth Santor
	@date 2014-09-29

	Vulcan calendar conversion implementations.
	*/

#include <khronos/vulcan_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	jd_t vulcan_to_jd(year_t year, month_t month, day_t day) {
		// Vulcan calendar: 12 months of 21 days each, with leap day in month 12 every 4 years
		// Total days per year: 252 (or 253 in leap years)

		long long total_days = 0;

		// Add days from complete years
		long long complete_years = year - 1;
		total_days += complete_years * 252;

		// Add leap days from complete years
		long long complete_leap_years = complete_years / 4;
		total_days += complete_leap_years;

		// Add days from complete months in current year
		for (int m = 1; m < month; ++m) {
			total_days += vulcan_days_in_month(m, is_vulcan_leapyear(year));
		}

		// Add days in current month
		total_days += day;

		return VULCAN_EPOCH + total_days - 1;
	}

	jd_t vulcan_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second) {
		jd_t jd = vulcan_to_jd(year, month, day);
		jd += tod(hour, minute, second);
		return jd;
	}

} // end-of-namespace khronos
