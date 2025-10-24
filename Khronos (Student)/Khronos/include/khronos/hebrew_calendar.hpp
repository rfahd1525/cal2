#pragma once

/**	@file khronos/hebrew_calendar.hpp
	@author Garth Santor
	@date 2013-09-26

	Hebrew calendar class.
	*/

#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>

namespace khronos {

	// VALUES
	constexpr jd_t HEBREW_EPOCH = 347'995.5;

	// OPERATIONS
	/** Leap year test for Hebrew Calendar.
	   The Hebrew calendar follows a 19-year Metonic cycle with leap years in years:
	   3, 6, 8, 11, 14, 17, and 19
	*/
	constexpr bool is_hebrew_leapyear(year_t year) {
		int cycle_year = ((year - 1) % 19) + 1;
		return (cycle_year == 3 || cycle_year == 6 || cycle_year == 8 || cycle_year == 11 ||
			cycle_year == 14 || cycle_year == 17 || cycle_year == 19);
	}

	/** Number of months in a Hebrew year (12 or 13) */
	constexpr int hebrew_months_in_year(year_t year) {
		return is_hebrew_leapyear(year) ? 13 : 12;
	}

	/** Get the day of year for Rosh Hashanah */
	long long hebrew_delay_1(year_t year);
	long long hebrew_delay_2(year_t year);
	long long hebrew_year_days(year_t year);
	long long hebrew_year_months(year_t year);

	/** Provide the number of days in the month of the Hebrew Calendar. */
	day_t hebrew_days_in_month(month_t month, year_t year);

	/** Provide the name of the given month in the Hebrew calendar. */
	char const* hebrew_month_name(month_t month, year_t year);

	// FUNCTIONS
	/** Convert Hebrew date to Julian Day Number. */
	jd_t hebrew_to_jd(year_t year, month_t month, day_t day);

	/** Convert Hebrew date and time to Julian Day. */
	jd_t hebrew_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day Number to Hebrew date. */
	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Hebrew date and time. */
	void jd_to_hebrew(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second);

	// CLASSES
	/**	Hebrew Calendar Date class. */
	class Hebrew {
		year_t year_;
		month_t month_;
		day_t day_;
		hour_t hour_;
		minute_t minute_;
		second_t second_;

		void from_jd(jd_t jd);

	public:
		// Constructors
		Hebrew();
		Hebrew(now_t now);
		Hebrew(jd_t jd);
		Hebrew(year_t year, month_t month, day_t day);
		Hebrew(year_t year, month_t month, day_t day,
			hour_t hour, minute_t minute, second_t second);

		// Accessors
		year_t year() const { return year_; }
		month_t month() const { return month_; }
		day_t day() const { return day_; }
		hour_t hour() const { return hour_; }
		minute_t minute() const { return minute_; }
		second_t second() const { return second_; }

		// Conversion
		jd_t to_jd() const;
		operator jd_t() const { return to_jd(); }
		std::string to_string() const;

		// Assignment
		Hebrew& operator=(jd_t jd);

		// Arithmetic operations
		Hebrew& operator+=(detail::packaged_year_real const& year);
		Hebrew& operator+=(detail::packaged_month_integer const& month);
		Hebrew operator+(detail::packaged_year_real const& year) const;
		Hebrew operator+(detail::packaged_month_integer const& month) const;
		Hebrew operator-(detail::packaged_month_integer const& month) const;
	};

	// Relational operators
	constexpr bool operator==(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() == rhs.to_jd();
	}

	constexpr auto operator<=>(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() <=> rhs.to_jd();
	}

	constexpr bool operator!=(Hebrew const& lhs, Hebrew const& rhs) {
		return !(lhs == rhs);
	}

	constexpr bool operator<(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() < rhs.to_jd();
	}

	constexpr bool operator<=(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() <= rhs.to_jd();
	}

	constexpr bool operator>(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() > rhs.to_jd();
	}

	constexpr bool operator>=(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() >= rhs.to_jd();
	}

	// Difference operator
	constexpr jd_t operator-(Hebrew const& lhs, Hebrew const& rhs) {
		return lhs.to_jd() - rhs.to_jd();
	}

} // end-of-namespace khronos
