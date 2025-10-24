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
	 * Algorithm based on Meeus "Astronomical Algorithms".
	 */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day) {
		// Calculate the number of complete Islamic years
		long long n = year - 1;

		// Calculate JD based on the 30-year cycle
		// 1 Islamic year ≈ 354.36667 days (average)
		// 30 Islamic years = 10631 days (11 leap years of 355 days + 19 normal years of 354 days)

		long long cycles = n / 30;
		long long yearInCycle = n % 30;

		long long days = cycles * 10631;  // Complete 30-year cycles

		// Add days for years in the current cycle
		days += yearInCycle * 354;

		// Add leap days for leap years in the current cycle
		for (long long y = 1; y <= yearInCycle; ++y) {
			if (is_islamic_leapyear(y)) {
				days++;
			}
		}

		// Add days for complete months
		for (month_t m = 1; m < month; ++m) {
			days += islamic_days_in_month(m, is_islamic_leapyear(year));
		}

		// Add the day
		days += day;

		// Convert to JD (subtract 1 because we counted the first day)
		return ISLAMIC_EPOCH + days - 1.0 - 0.5;
	}

	/**
	 * Convert Islamic date and time to Julian Day.
	 */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day,
	                    hour_t hour, minute_t minute, second_t second) {
		jd_t jd = islamic_to_jd(year, month, day);
		jd += tod(hour, minute, second);
		return jd;
	}

	/**
	 * Convert Julian Day to Islamic calendar date.
	 */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day) {
		long long jdn = static_cast<long long>(std::floor(jd + 0.5));

		// Days since Islamic epoch
		long long daysSinceEpoch = jdn - static_cast<long long>(ISLAMIC_EPOCH + 0.5) + 1;

		// Estimate the year using 30-year cycles
		long long cycles = daysSinceEpoch / 10631;
		long long daysInCycle = daysSinceEpoch % 10631;

		// Find the year within the cycle
		year_t y = 1;
		long long daysAccum = 0;

		while (daysInCycle > daysAccum + (is_islamic_leapyear(y) ? 355 : 354)) {
			daysAccum += (is_islamic_leapyear(y) ? 355 : 354);
			y++;
		}

		year = static_cast<year_t>(cycles * 30 + y);

		// Days remaining in this year
		long long daysInYear = daysInCycle - daysAccum;

		// Find the month
		month = 1;
		while (daysInYear > islamic_days_in_month(month, is_islamic_leapyear(year))) {
			daysInYear -= islamic_days_in_month(month, is_islamic_leapyear(year));
			month++;
		}

		// Remaining days is the day of month
		day = static_cast<day_t>(daysInYear);
	}

	/**
	 * Convert Julian Day to Islamic calendar date and time.
	 */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day,
	                    hour_t& hour, minute_t& minute, second_t& second) {
		jd_to_islamic(jd, year, month, day);

		double jdn = std::floor(jd + 0.5);
		tod_t timeOfDay = jd - (jdn - 0.5);

		jd_to_hms(timeOfDay, hour, minute, second);
	}

} // end-of-namespace khronos
