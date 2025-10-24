#pragma once

/**	@file vulcan.hpp
	@author Garth Santor
	@date 2014-09-29

	Vulcan calendar class and functions.
	*/

#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>

namespace khronos {

	// VALUES
	constexpr jd_t VULCAN_EPOCH = 1'723'762.5;

	// OPERATIONS
	/** Leap year test for Vulcan Calendar. */
	constexpr bool is_vulcan_leapyear(year_t year) {
		return year % 4 == 0;
	}

	/** Provide the number of days in the month of the Vulcan Calendar. */
	constexpr day_t vulcan_days_in_month(month_t month, bool isLeapYear) {
		if (month <= 0 || month > 12) return 0;
		if (month == 12 && isLeapYear) return 22;
		return 21;
	}

	/** Provide the name of the given month in the Vulcan calendar. */
	constexpr char const* vulcan_month_name(month_t month) {
		constexpr char const* const names[13] = {
			"",
			"Z'at", "D'ruh", "K'riBrax", "re'T'Khutai", "T'keKhuti", "Khuti",
			"Ta'Krat", "K'ri'lior", "et'Khior", "T'lakht", "T'ke'Tas", "Tasmeen"
		};
		return names[month];
	}

	// FUNCTIONS
	/** Convert Vulcan date to Julian Day Number. */
	jd_t vulcan_to_jd(year_t year, month_t month, day_t day);

	/** Convert Vulcan date and time to Julian Day. */
	jd_t vulcan_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day Number to Vulcan date. */
	void jd_to_vulcan(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Vulcan date and time. */
	void jd_to_vulcan(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second);

	// CLASSES
	/**	Vulcan Calendar Date class. */
	class Vulcan {
		year_t year_;
		month_t month_;
		day_t day_;
		hour_t hour_;
		minute_t minute_;
		second_t second_;

		void from_jd(jd_t jd);

	public:
		// Constructors
		Vulcan();
		Vulcan(now_t now);
		Vulcan(jd_t jd);
		Vulcan(year_t year, month_t month, day_t day);
		Vulcan(year_t year, month_t month, day_t day,
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
		Vulcan& operator=(jd_t jd);

		// Arithmetic operations
		Vulcan& operator+=(detail::packaged_year_real const& year);
		Vulcan& operator+=(detail::packaged_month_integer const& month);
		Vulcan operator+(detail::packaged_year_real const& year) const;
		Vulcan operator+(detail::packaged_month_integer const& month) const;
		Vulcan operator-(detail::packaged_month_integer const& month) const;
	};

	// Relational operators
	constexpr bool operator==(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() == rhs.to_jd();
	}

	constexpr auto operator<=>(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() <=> rhs.to_jd();
	}

	constexpr bool operator!=(Vulcan const& lhs, Vulcan const& rhs) {
		return !(lhs == rhs);
	}

	constexpr bool operator<(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() < rhs.to_jd();
	}

	constexpr bool operator<=(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() <= rhs.to_jd();
	}

	constexpr bool operator>(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() > rhs.to_jd();
	}

	constexpr bool operator>=(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() >= rhs.to_jd();
	}

	// Difference operator
	constexpr jd_t operator-(Vulcan const& lhs, Vulcan const& rhs) {
		return lhs.to_jd() - rhs.to_jd();
	}

} // end-of-namespace khronos
