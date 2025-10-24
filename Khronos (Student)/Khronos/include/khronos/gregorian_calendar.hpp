#pragma once
/*
khronos\gregorian_calendar.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'Gregorian calendar' declarations.
*/


#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>


namespace khronos {
	// LITERALS
	/* UDL - converts a Gregorian year CE to an astronomical Gregorian year. */
	constexpr year_t operator ""_CE(unsigned long long year) {
		return static_cast<year_t>(year);
	}

	/* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	constexpr year_t operator ""_BCE(unsigned long long year) {
		return -static_cast<year_t>(year) + 1;
	}

	// VALUES
	constexpr jd_t GREGORIAN_EPOCH = 1'721'425.5;

	// OPERATIONS
	/**	Leap year test for Proleptic Gregorian Calendar.
		@return 'true' if leap year, 'false' if not.
		@param year [in] Astronomical Gregorian year (1 CE = 1, 1 BCE = 0, 2 BCE = -1, etc.)
		*/
	constexpr bool is_gregorian_leapyear(year_t year) {
		return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	}

	/** Provide the number of days in the month of the Gregorian Calendar. */
	constexpr day_t gregorian_days_in_month(month_t month, bool isLeapYear) {
		return civil::days_in_month(month, isLeapYear);
	}

	/** Provide the name of the given month in the Gregorian calendar. */
	constexpr char const * gregorian_month_name(month_t month) {
		return civil::month_name_long(month);
	}

	/** Provide the short name of the given month in the Gregorian calendar. */
	constexpr char const * gregorian_short_month_name(month_t month) {
		return civil::month_name_short(month);
	}

	// FUNCTIONS
	/** Convert Gregorian date to Julian Day Number (JD). */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day);

	/** Convert Gregorian date and time to Julian Day (JD with fractional part). */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day Number to Gregorian date. */
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Gregorian date and time. */
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day,
		hour_t& hour, minute_t& minute, second_t& second);

	// CLASSES
	// --------------------------------------------------------------------------------------

	/**	Proleptic Gregorian Calendar Date class. */
	class Gregorian {
		year_t year_;
		month_t month_;
		day_t day_;
		hour_t hour_;
		minute_t minute_;
		second_t second_;

		void from_jd(jd_t jd);

	public:
		// Constructors
		Gregorian();
		Gregorian(now_t now);
		Gregorian(jd_t jd);
		Gregorian(year_t year, month_t month, day_t day);
		Gregorian(year_t year, month_t month, day_t day,
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
		Gregorian& operator=(jd_t jd);

		// Arithmetic operations
		Gregorian& operator+=(detail::packaged_year_real const& year);
		Gregorian& operator+=(detail::packaged_month_integer const& month);
		Gregorian operator+(detail::packaged_year_real const& year) const;
		Gregorian operator+(detail::packaged_month_integer const& month) const;
		Gregorian operator-(detail::packaged_month_integer const& month) const;
	};

	// Relational operators
	constexpr bool operator==(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() == rhs.to_jd();
	}

	constexpr auto operator<=>(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() <=> rhs.to_jd();
	}

	constexpr bool operator!=(Gregorian const& lhs, Gregorian const& rhs) {
		return !(lhs == rhs);
	}

	constexpr bool operator<(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() < rhs.to_jd();
	}

	constexpr bool operator<=(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() <= rhs.to_jd();
	}

	constexpr bool operator>(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() > rhs.to_jd();
	}

	constexpr bool operator>=(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() >= rhs.to_jd();
	}

	// Difference operator
	constexpr jd_t operator-(Gregorian const& lhs, Gregorian const& rhs) {
		return lhs.to_jd() - rhs.to_jd();
	}

	// OPERATORS
	// ====================

} // end-of-namespace khronos
