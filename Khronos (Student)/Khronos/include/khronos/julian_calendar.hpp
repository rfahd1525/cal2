#pragma once

/**	@file khronos/julian_calendar.hpp
	@author Garth Santor
	@date 2013-09-19

	Julian calendar functions and class declaration.
	*/

#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>

namespace khronos {

	// VALUES
	constexpr jd_t JULIAN_EPOCH = 1'721'423.5;

	// OPERATIONS
	/**	Leap year test for Proleptic Julian Calendar.
		@return 'true' if leap year, 'false' if not.
		@param year [in] Astronomical Julian year
		*/
	constexpr bool is_julian_leapyear(year_t year) {
		return year % 4 == 0;
	}

	/** Provide the number of days in the month of the Julian Calendar. */
	constexpr day_t julian_days_in_month(month_t month, bool isLeapYear) {
		return civil::days_in_month(month, isLeapYear);
	}

	/** Provide the name of the given month in the Julian calendar. */
	constexpr char const * julian_month_name(month_t month) {
		return civil::month_name_long(month);
	}

	/** Provide the short name of the given month in the Julian calendar. */
	constexpr char const * julian_short_month_name(month_t month) {
		return civil::month_name_short(month);
	}

	// FUNCTIONS
	/** Convert Julian calendar date to Julian Day Number. */
	jd_t julian_to_jd(year_t year, month_t month, day_t day);

	/** Convert Julian calendar date and time to Julian Day. */
	jd_t julian_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day Number to Julian calendar date. */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Julian calendar date and time. */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second);

	// CLASSES
	/**	Proleptic Julian Calendar Date class. */
	class Julian {
		year_t year_;
		month_t month_;
		day_t day_;
		hour_t hour_;
		minute_t minute_;
		second_t second_;

		void from_jd(jd_t jd);

	public:
		// Constructors
		Julian();
		Julian(now_t now);
		Julian(jd_t jd);
		Julian(year_t year, month_t month, day_t day);
		Julian(year_t year, month_t month, day_t day,
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
		Julian& operator=(jd_t jd);

		// Arithmetic operations
		Julian& operator+=(detail::packaged_year_real const& year);
		Julian& operator+=(detail::packaged_month_integer const& month);
		Julian operator+(detail::packaged_year_real const& year) const;
		Julian operator+(detail::packaged_month_integer const& month) const;
		Julian operator-(detail::packaged_month_integer const& month) const;
	};

	// Relational operators
	constexpr bool operator==(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() == rhs.to_jd();
	}

	constexpr auto operator<=>(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() <=> rhs.to_jd();
	}

	constexpr bool operator!=(Julian const& lhs, Julian const& rhs) {
		return !(lhs == rhs);
	}

	constexpr bool operator<(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() < rhs.to_jd();
	}

	constexpr bool operator<=(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() <= rhs.to_jd();
	}

	constexpr bool operator>(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() > rhs.to_jd();
	}

	constexpr bool operator>=(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() >= rhs.to_jd();
	}

	// Difference operator
	constexpr jd_t operator-(Julian const& lhs, Julian const& rhs) {
		return lhs.to_jd() - rhs.to_jd();
	}

} // end-of-namespace khronos
