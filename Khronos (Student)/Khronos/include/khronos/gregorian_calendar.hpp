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
	// TYPE ALIASES
	using jd_t = double;

	// LITERALS
	/* UDL - converts a Gregorian year CE to an astronomical Gregorian year. */
	// Defined in timeofday.hpp

	/* UDL - converts a Gregorian year BCE to an astronomical Gregorian year. */
	// Defined in timeofday.hpp

	// VALUES
	constexpr jd_t GREGORIAN_EPOCH = 1721425.5;

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

	// CONVERSION FUNCTIONS
	/** Convert Gregorian date to Julian Day Number. */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day);

	/** Convert Gregorian date and time to Julian Day. */
	jd_t gregorian_to_jd(year_t year, month_t month, day_t day,
	                      hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day to Gregorian date. */
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Gregorian date and time. */
	void jd_to_gregorian(jd_t jd, year_t& year, month_t& month, day_t& day,
	                      hour_t& hour, minute_t& minute, second_t& second);

	// FORWARD DECLARATIONS
	class Jd;

	// CLASSES
	// --------------------------------------------------------------------------------------

	/**	Proleptic Gregorian Calendar Date class. */
	class Gregorian {
		jd_t jd_;

	public:
		// Constructors
		Gregorian();  // defaults to current date/time
		explicit Gregorian(now_t nowType);  // now with or without time
		Gregorian(year_t year, month_t month, day_t day);
		Gregorian(year_t year, month_t month, day_t day,
		          hour_t hour, minute_t minute, second_t second);
		explicit Gregorian(Jd const& jd);

		// Accessors
		year_t year() const;
		month_t month() const;
		day_t day() const;
		hour_t hour() const;
		minute_t minute() const;
		second_t second() const;

		// Conversion
		operator Jd() const;
		Gregorian& operator=(Jd const& jd);

		// Arithmetic
		Gregorian operator+(days const& d) const;
		Gregorian operator-(days const& d) const;
		Gregorian operator+(weeks const& w) const;
		Gregorian operator-(weeks const& w) const;
		Gregorian operator+(months const& m) const;
		Gregorian operator-(months const& m) const;
		Gregorian operator+(years const& y) const;
		Gregorian operator-(years const& y) const;

		jd_t operator-(Gregorian const& rhs) const { return jd_ - rhs.jd_; }

		// Relational operators
		bool operator==(Gregorian const& rhs) const { return jd_ == rhs.jd_; }
		bool operator!=(Gregorian const& rhs) const { return jd_ != rhs.jd_; }
		bool operator<(Gregorian const& rhs) const { return jd_ < rhs.jd_; }
		bool operator<=(Gregorian const& rhs) const { return jd_ <= rhs.jd_; }
		bool operator>(Gregorian const& rhs) const { return jd_ > rhs.jd_; }
		bool operator>=(Gregorian const& rhs) const { return jd_ >= rhs.jd_; }

		// String conversion
		std::string to_string() const;
	};

	// Helper classes for arithmetic
	class days {
		double days_;
	public:
		explicit days(double d) : days_(d) {}
		double value() const { return days_; }
	};

	class weeks {
		double weeks_;
	public:
		explicit weeks(double w) : weeks_(w) {}
		double value() const { return weeks_; }
	};

	class months {
		int months_;
	public:
		explicit months(int m) : months_(m) {}
		int value() const { return months_; }
	};

	class years {
		double years_;
	public:
		explicit years(double y) : years_(y) {}
		double value() const { return years_; }
	};

	// OPERATORS
	// ====================
	std::ostream& operator<<(std::ostream& os, Gregorian const& g);

	// Day of week function
	enum day_of_week_enum { Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };
	day_of_week_enum day_of_week(Gregorian const& g);

} // end-of-namespace khronos
