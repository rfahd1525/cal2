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
	constexpr jd_t JULIAN_EPOCH = 1721423.5;

	// OPERATIONS
	/**	Leap year test for Proleptic Julian Calendar.
		Julian calendar: every 4th year is a leap year.
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
	constexpr char const* julian_month_name(month_t month) {
		return civil::month_name_long(month);
	}

	/** Provide the short name of the given month in the Julian calendar. */
	constexpr char const* julian_short_month_name(month_t month) {
		return civil::month_name_short(month);
	}

	// CONVERSION FUNCTIONS
	/** Convert Julian date to Julian Day Number. */
	jd_t julian_to_jd(year_t year, month_t month, day_t day);

	/** Convert Julian date and time to Julian Day. */
	jd_t julian_to_jd(year_t year, month_t month, day_t day,
	                   hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day to Julian calendar date. */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Julian calendar date and time. */
	void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day,
	                   hour_t& hour, minute_t& minute, second_t& second);

	// FORWARD DECLARATIONS
	class Jd;

	// CLASSES
	/**	Proleptic Julian Calendar Date class. */
	class Julian {
		jd_t jd_;

	public:
		// Constructors
		Julian();
		explicit Julian(now_t nowType);
		Julian(year_t year, month_t month, day_t day);
		Julian(year_t year, month_t month, day_t day,
		        hour_t hour, minute_t minute, second_t second);
		explicit Julian(Jd const& jd);

		// Accessors
		year_t year() const;
		month_t month() const;
		day_t day() const;
		hour_t hour() const;
		minute_t minute() const;
		second_t second() const;

		// Conversion
		operator Jd() const;
		Julian& operator=(Jd const& jd);

		// Arithmetic
		Julian operator+(days const& d) const;
		Julian operator-(days const& d) const;
		Julian operator+(weeks const& w) const;
		Julian operator-(weeks const& w) const;
		Julian operator+(months const& m) const;
		Julian operator-(months const& m) const;
		Julian operator+(years const& y) const;
		Julian operator-(years const& y) const;

		jd_t operator-(Julian const& rhs) const { return jd_ - rhs.jd_; }

		// Relational operators
		bool operator==(Julian const& rhs) const { return jd_ == rhs.jd_; }
		bool operator!=(Julian const& rhs) const { return jd_ != rhs.jd_; }
		bool operator<(Julian const& rhs) const { return jd_ < rhs.jd_; }
		bool operator<=(Julian const& rhs) const { return jd_ <= rhs.jd_; }
		bool operator>(Julian const& rhs) const { return jd_ > rhs.jd_; }
		bool operator>=(Julian const& rhs) const { return jd_ >= rhs.jd_; }

		// String conversion
		std::string to_string() const;
	};

	// Stream operator
	std::ostream& operator<<(std::ostream& os, Julian const& j);

	// Day of week function
	day_of_week_enum day_of_week(Julian const& j);

} // end-of-namespace khronos
