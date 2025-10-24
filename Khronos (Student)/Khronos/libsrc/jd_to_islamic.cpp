/**	@file jd_to_islamic.cpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar conversion from JD implementations.
	*/

#include <khronos/islamic_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	/**
	 * Convert Julian Day Number to Islamic calendar date.
	 */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day) {
		// Calculate days since Islamic epoch
		long long days = static_cast<long long>(std::floor(jd - ISLAMIC_EPOCH + 1));

		// Calculate complete 30-year cycles
		long long cycles = days / 10631;
		long long remaining_days = days % 10631;

		// Calculate year within the cycle
		year_t year_in_cycle = 1;
		while (remaining_days > 0) {
			int year_length = 354 + (is_islamic_leapyear(year_in_cycle) ? 1 : 0);
			if (remaining_days >= year_length) {
				remaining_days -= year_length;
				year_in_cycle++;
			}
			else {
				break;
			}
		}

		year = cycles * 30 + year_in_cycle;

		// Calculate month and day
		bool is_leap = is_islamic_leapyear(year);
		month = 1;
		while (month < 12) {
			int month_length = islamic_days_in_month(month, is_leap);
			if (remaining_days >= month_length) {
				remaining_days -= month_length;
				month++;
			}
			else {
				break;
			}
		}

		day = static_cast<day_t>(remaining_days + 1);
	}

	/**
	 * Convert Julian Day to Islamic calendar date and time.
	 */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second) {
		jd_to_islamic(jd, year, month, day);

		double jd_floor = std::floor(jd + 0.5);
		double tod_fraction = jd - (jd_floor - 0.5);

		if (tod_fraction < 0) tod_fraction += 1.0;
		if (tod_fraction >= 1.0) tod_fraction -= 1.0;

		jd_to_hms(tod_fraction, hour, minute, second);
	}

} // end-of-namespace khronos
