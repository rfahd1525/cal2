/**	@file Hebrew_to_jd.cpp
	@author Garth Santor
	@date 2012-09-14

	Hebrew calendar conversion implementations.
	*/

#include <khronos/hebrew_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>

namespace khronos {

	// Hebrew calendar helper functions
	long long hebrew_delay_1(year_t year) {
		long long months = ((235 * year - 234) / 19);
		long long parts = 12084 + 13753 * months;
		long long day = months * 29 + parts / 25920;

		if ((3 * (day + 1)) % 7 < 3) {
			day++;
		}
		return day;
	}

	long long hebrew_delay_2(year_t year) {
		long long last = hebrew_delay_1(year - 1);
		long long present = hebrew_delay_1(year);
		long long next = hebrew_delay_1(year + 1);

		if (next - present == 356) {
			return 2;
		}
		else if (present - last == 382) {
			return 1;
		}
		return 0;
	}

	long long hebrew_year_days(year_t year) {
		return hebrew_delay_1(year + 1) - hebrew_delay_1(year);
	}

	long long hebrew_year_months(year_t year) {
		return is_hebrew_leapyear(year) ? 13 : 12;
	}

	day_t hebrew_days_in_month(month_t month, year_t year) {
		bool is_leap = is_hebrew_leapyear(year);
		long long year_length = hebrew_year_days(year);

		switch (month) {
		case 1:  // Nisan
		case 3:  // Sivan
		case 5:  // Av
		case 7:  // Tishrei
		case 11: // Shevat
			return 30;
		case 2:  // Iyyar
		case 4:  // Tammuz
		case 6:  // Elul
		case 10: // Tevet
			return 29;
		case 8:  // Cheshvan
			return (year_length == 355 || year_length == 385) ? 30 : 29;
		case 9:  // Kislev
			return (year_length == 353 || year_length == 383) ? 29 : 30;
		case 12: // Adar (or Adar I in leap years)
			return is_leap ? 30 : 29;
		case 13: // Adar II (only in leap years)
			return is_leap ? 29 : 0;
		default:
			return 0;
		}
	}

	char const* hebrew_month_name(month_t month, year_t year) {
		static char const* const names[14] = {
			"",
			"Nisan", "Iyyar", "Sivan", "Tammuz", "Av", "Elul",
			"Tishrei", "Cheshvan", "Kislev", "Tevet", "Shevat",
			"Adar", "Adar II"
		};

		if (month == 12 && is_hebrew_leapyear(year)) {
			return "Adar I";
		}
		return names[month];
	}

	jd_t hebrew_to_jd(year_t year, month_t month, day_t day) {
		long long months_elapsed;
		if (month < 7) {
			months_elapsed = month - 1;
			for (int m = 7; m <= hebrew_year_months(year); ++m) {
				months_elapsed += hebrew_days_in_month(m, year);
			}
		}
		else {
			months_elapsed = 0;
			for (int m = 7; m < month; ++m) {
				months_elapsed += hebrew_days_in_month(m, year);
			}
		}

		long long day_of_year = hebrew_delay_1(year) + hebrew_delay_2(year) + months_elapsed + day;

		return HEBREW_EPOCH + day_of_year - 1;
	}

	jd_t hebrew_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second) {
		jd_t jd = hebrew_to_jd(year, month, day);
		jd += tod(hour, minute, second);
		return jd;
	}

} // end-of-namespace khronos
