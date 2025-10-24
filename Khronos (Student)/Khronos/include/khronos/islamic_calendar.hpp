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
	constexpr jd_t ISLAMIC_EPOCH = 1948439.5;

	// Islamic month names
	enum islamic_months {
		Muharram = 1, Safar, RabialAwwal, RabiathThani,
		JumadaIUla, JumadatTania, Rajab, Shaban,
		Ramadan, Shawwal, DhulQadah, DhulHijja
	};

	// OPERATIONS
	/**	Leap year test for Islamic Calendar.
		Islamic calendar has 11 leap years in a 30-year cycle.
		Leap years in the cycle: 2, 5, 7, 10, 13, 16, 18, 21, 24, 26, 29
		@return 'true' if leap year, 'false' if not.
		@param year [in] Islamic year
		*/
	constexpr bool is_islamic_leapyear(year_t year) {
		// Handle negative years by adjusting to positive equivalent in the cycle
		year_t yearInCycle = ((year % 30) + 30) % 30;
		return yearInCycle == 2 || yearInCycle == 5 || yearInCycle == 7 ||
		       yearInCycle == 10 || yearInCycle == 13 || yearInCycle == 16 ||
		       yearInCycle == 18 || yearInCycle == 21 || yearInCycle == 24 ||
		       yearInCycle == 26 || yearInCycle == 29;
	}

	/** Provide the number of days in the month of the Islamic Calendar. */
	constexpr day_t islamic_days_in_month(month_t month, bool isLeapYear) {
		// Islamic months alternate 30 and 29 days
		// Odd months (1,3,5,7,9,11) have 30 days
		// Even months (2,4,6,8,10) have 29 days
		// Month 12 has 29 days normally, 30 in leap years
		if (month == 12) {
			return isLeapYear ? 30 : 29;
		}
		return (month % 2 == 1) ? 30 : 29;
	}

	/** Provide the name of the given month in the Islamic calendar. */
	constexpr char const* islamic_month_name(month_t month) {
		constexpr char const* const names[13] = {
			"",
			"Muharram", "Safar", "Rabi'al-Awwal", "Rabi'ath-Thani",
			"Jumada I-Ula", "Jumada t-Tania", "Rajab", "Sha'ban",
			"Ramadan", "Shawwal", "Dhu I-Qa'da", "Dhu I-Hijja"
		};
		return names[month];
	}

	// CONVERSION FUNCTIONS
	/** Convert Islamic date to Julian Day Number. */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day);

	/** Convert Islamic date and time to Julian Day. */
	jd_t islamic_to_jd(year_t year, month_t month, day_t day,
	                    hour_t hour, minute_t minute, second_t second);

	/** Convert Julian Day to Islamic calendar date. */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day);

	/** Convert Julian Day to Islamic calendar date and time. */
	void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day,
	                    hour_t& hour, minute_t& minute, second_t& second);

	// FORWARD DECLARATIONS
	class Jd;

	// CLASSES
	/**	Islamic Calendar Date class. */
	class Islamic {
		jd_t jd_;

	public:
		// Constructors
		Islamic();
		explicit Islamic(now_t nowType);
		Islamic(year_t year, month_t month, day_t day);
		Islamic(year_t year, month_t month, day_t day,
		         hour_t hour, minute_t minute, second_t second);
		explicit Islamic(Jd const& jd);

		// Accessors
		year_t year() const;
		month_t month() const;
		day_t day() const;
		hour_t hour() const;
		minute_t minute() const;
		second_t second() const;

		// Conversion
		operator Jd() const;
		Islamic& operator=(Jd const& jd);

		// Arithmetic
		Islamic operator+(days const& d) const;
		Islamic operator-(days const& d) const;
		Islamic operator+(weeks const& w) const;
		Islamic operator-(weeks const& w) const;
		Islamic operator+(months const& m) const;
		Islamic operator-(months const& m) const;
		Islamic operator+(years const& y) const;
		Islamic operator-(years const& y) const;

		jd_t operator-(Islamic const& rhs) const { return jd_ - rhs.jd_; }

		// Relational operators
		bool operator==(Islamic const& rhs) const { return jd_ == rhs.jd_; }
		bool operator!=(Islamic const& rhs) const { return jd_ != rhs.jd_; }
		bool operator<(Islamic const& rhs) const { return jd_ < rhs.jd_; }
		bool operator<=(Islamic const& rhs) const { return jd_ <= rhs.jd_; }
		bool operator>(Islamic const& rhs) const { return jd_ > rhs.jd_; }
		bool operator>=(Islamic const& rhs) const { return jd_ >= rhs.jd_; }

		// String conversion
		std::string to_string() const;
	};

	// Stream operator
	std::ostream& operator<<(std::ostream& os, Islamic const& i);

	// Day of week function
	day_of_week_enum day_of_week(Islamic const& i);

} // end-of-namespace khronos
