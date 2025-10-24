/**	@file jd_to_vulcan.cpp
	@author Garth Santor
	@date 2014-09-29

	Vulcan calendar conversion from JD implementations.
	*/

#include <khronos/vulcan_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	void jd_to_vulcan(jd_t jd, year_t& year, month_t& month, day_t& day) {
		long long days = static_cast<long long>(std::floor(jd - VULCAN_EPOCH + 1));

		// Approximate year (252 days per year, plus leap days)
		year = static_cast<year_t>((days * 4) / (252 * 4 + 1)) + 1;

		// Calculate start of current year
		long long year_start = (year - 1) * 252 + (year - 1) / 4;

		// Adjust if necessary
		while (year_start > days) {
			year--;
			year_start = (year - 1) * 252 + (year - 1) / 4;
		}

		while (year_start + 252 + (is_vulcan_leapyear(year) ? 1 : 0) <= days) {
			year_start += 252 + (is_vulcan_leapyear(year) ? 1 : 0);
			year++;
		}

		// Calculate days remaining in current year
		long long day_of_year = days - year_start + 1;

		// Determine month and day
		month = 1;
		bool is_leap = is_vulcan_leapyear(year);
		while (month < 12) {
			int month_days = vulcan_days_in_month(month, is_leap);
			if (day_of_year <= month_days) {
				break;
			}
			day_of_year -= month_days;
			month++;
		}

		day = static_cast<day_t>(day_of_year);
	}

	void jd_to_vulcan(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second) {
		jd_to_vulcan(jd, year, month, day);

		double jd_floor = std::floor(jd + 0.5);
		double tod_fraction = jd - (jd_floor - 0.5);

		if (tod_fraction < 0) tod_fraction += 1.0;
		if (tod_fraction >= 1.0) tod_fraction -= 1.0;

		jd_to_hms(tod_fraction, hour, minute, second);
	}

} // end-of-namespace khronos
