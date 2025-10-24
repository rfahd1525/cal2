#pragma once

/**	@file khronos/Islamic_calendar.hpp
	@author Garth Santor
	@date 2013-09-19

	Islamic calendar class and functions.
	*/

#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>

namespace khronos {

	// VALUES
	constexpr jd_t ISLAMIC_EPOCH = 1'948'439.5;

	// OPERATIONS
	/**	Leap year test for Islamic Calendar.
		The Islamic calendar follows a 30-year cycle where leap years occur in years:
		2, 5, 7, 10, 13, 16, 18, 21, 24, 26, and 29
		*/
	constexpr bool is_islamic_leapyear(year_t year) {
		int cycle_year = ((year - 1) % 30);
		if (cycle_year < 0) cycle_year += 30;
		cycle_year++;
		return (cycle_year == 2 || cycle_year == 5 || cycle_year == 7 || cycle_year == 10 ||
			cycle_year == 13 || cycle_year == 16 || cycle_year == 18 || cycle_year == 21 ||
			cycle_year == 24 || cycle_year == 26 || cycle_year == 29);
	}

	/** Provide the number of days in the month of the Islamic Calendar. */
	constexpr day_t islamic_days_in_month(month_t month, bool isLeapYear) {
		// Islamic months alternate between 30 and 29 days
		// Month 12 has 30 days in leap years instead of 29
		if (month <= 0 || month > 12) return 0;
		if (month % 2 == 1) return 30;  // Odd months have 30 days
		if (month == 12 && isLeapYear) return 30;  // Dhu al-Hijjah has 30 days in leap years
		return 29;  // Even months have 29 days
	}

	/** Provide the name of the given month in the Islamic calendar. */
	constexpr char const * islamic_month_name(month_t month) {
		constexpr char const* const names[13] = {
			"",
			"Muharram", "Safar", "Rabi'al-Awwal",
			"Rabi'ath-Thani", "Jumada I-Ula", "Jumada t-Tania",
			"Rajab", "Sha'ban", "Ramadan", "Shawwal",
			"Dhu I-Qa'da", "Dhu I-Hijja"
		};
		return names[month];
	}

	// FUNCTIONS
	/** Convert Islamic date to Julian Day Number. */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day);

	/** Convert Islamic date and time to Julian Day. */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day Number to Islamic date. */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Islamic date and time. */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second);

	// CLASSES
	/**	Islamic Calendar Date class. */
	class Islamic {
		year_t year_;
		month_t month_;
		day_t day_;
		hour_t hour_;
		minute_t minute_;
		second_t second_;

		void from_jd(jd_t jd);

	public:
		// Constructors
		Islamic();
		Islamic(now_t now);
		Islamic(jd_t jd);
		Islamic(year_t year, month_t month, day_t day);
		Islamic(year_t year, month_t month, day_t day,
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
		Islamic& operator=(jd_t jd);

		// Arithmetic operations
		Islamic& operator+=(detail::packaged_year_real const& year);
		Islamic& operator+=(detail::packaged_month_integer const& month);
		Islamic operator+(detail::packaged_year_real const& year) const;
		Islamic operator+(detail::packaged_month_integer const& month) const;
		Islamic operator-(detail::packaged_month_integer const& month) const;
	};

	// Relational operators
	constexpr bool operator==(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() == rhs.to_jd();
	}

	constexpr auto operator<=>(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() <=> rhs.to_jd();
	}

	constexpr bool operator!=(Islamic const& lhs, Islamic const& rhs) {
		return !(lhs == rhs);
	}

	constexpr bool operator<(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() < rhs.to_jd();
	}

	constexpr bool operator<=(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() <= rhs.to_jd();
	}

	constexpr bool operator>(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() > rhs.to_jd();
	}

	constexpr bool operator>=(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() >= rhs.to_jd();
	}

	// Difference operator
	constexpr jd_t operator-(Islamic const& lhs, Islamic const& rhs) {
		return lhs.to_jd() - rhs.to_jd();
	}

} // end-of-namespace khronos
