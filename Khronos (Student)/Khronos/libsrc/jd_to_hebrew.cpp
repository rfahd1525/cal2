/**	@file jd_to_hebrew.cpp
	@author Garth Santor
	@date 2012-09-14

	Hebrew calendar conversion from JD implementations.
	*/

#include <khronos/hebrew_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day) {
		long long d = static_cast<long long>(std::floor(jd - HEBREW_EPOCH + 1));

		// Approximate year
		year = static_cast<year_t>(d / 366);

		// Refine year
		while (d >= hebrew_delay_1(year + 1)) {
			year++;
		}

		// Calculate days from start of year
		long long year_start = hebrew_delay_1(year) + hebrew_delay_2(year);
		long long days_remaining = d - year_start + 1;

		// Determine if we're before or after Tishrei
		long long days_from_tishrei = 0;
		month = 7; // Start from Tishrei

		// Count through months starting from Tishrei
		int total_months = hebrew_year_months(year);
		for (int m = 7; m <= total_months; ++m) {
			int month_days = hebrew_days_in_month(m, year);
			if (days_remaining <= month_days) {
				month = m;
				day = static_cast<day_t>(days_remaining);
				return;
			}
			days_remaining -= month_days;
		}

		// Continue with months 1-6
		for (int m = 1; m <= 6; ++m) {
			int month_days = hebrew_days_in_month(m, year);
			if (days_remaining <= month_days) {
				month = m;
				day = static_cast<day_t>(days_remaining);
				return;
			}
			days_remaining -= month_days;
		}

		// Fallback
		month = 1;
		day = 1;
	}

	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second) {
		jd_to_hebrew(jd, year, month, day);

		double jd_floor = std::floor(jd + 0.5);
		double tod_fraction = jd - (jd_floor - 0.5);

		if (tod_fraction < 0) tod_fraction += 1.0;
		if (tod_fraction >= 1.0) tod_fraction -= 1.0;

		jd_to_hms(tod_fraction, hour, minute, second);
	}

} // end-of-namespace khronos
