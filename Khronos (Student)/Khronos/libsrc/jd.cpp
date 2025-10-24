/**	@file Jdn.cpp
	@author Garth Santor
	@date 2012-09-16

	Date and time library to_string method implementations.
	*/


#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace khronos {

	// Default constructor - initializes to current time
	Jd::Jd() {
		time_t now = time(nullptr);
		struct tm local;
		localtime_s(&local, &now);
		jd_ = gregorian_to_jd(
			year_t(local.tm_year) + 1900,
			month_t(local.tm_mon) + 1,
			day_t(local.tm_mday),
			hour_t(local.tm_hour),
			minute_t(local.tm_min),
			second_t(local.tm_sec)
		);
	}

	// Constructor with now_t parameter
	Jd::Jd(now_t nowType) {
		time_t now = time(nullptr);
		struct tm local;
		localtime_s(&local, &now);

		if (nowType == NOTIMEOFDAY) {
			// No time of day, just the date at midnight
			jd_ = gregorian_to_jd(
				year_t(local.tm_year) + 1900,
				month_t(local.tm_mon) + 1,
				day_t(local.tm_mday)
			);
		} else {
			// With time of day
			jd_ = gregorian_to_jd(
				year_t(local.tm_year) + 1900,
				month_t(local.tm_mon) + 1,
				day_t(local.tm_mday),
				hour_t(local.tm_hour),
				minute_t(local.tm_min),
				second_t(local.tm_sec)
			);
		}
	}

	// Arithmetic operators
	Jd Jd::operator+(days const& d) const {
		return Jd(jd_ + d.value());
	}

	Jd Jd::operator-(days const& d) const {
		return Jd(jd_ - d.value());
	}

	Jd Jd::operator+(weeks const& w) const {
		return Jd(jd_ + w.value() * 7.0);
	}

	Jd Jd::operator-(weeks const& w) const {
		return Jd(jd_ - w.value() * 7.0);
	}

	Jd Jd::operator+(years const& y) const {
		return Jd(jd_ + y.value() * EARTH_ORBITAL_PERIOD_DAYS);
	}

	Jd Jd::operator-(years const& y) const {
		return Jd(jd_ - y.value() * EARTH_ORBITAL_PERIOD_DAYS);
	}

	// String conversion
	std::string Jd::to_string() const {
		std::ostringstream oss;
		oss << "JD " << jd_;
		return oss.str();
	}

	// Stream operator
	std::ostream& operator<<(std::ostream& os, Jd const& jd) {
		return os << jd.to_string();
	}

	// Day of week function for Jd
	day_of_week_enum day_of_week(Jd const& jd) {
		return static_cast<day_of_week_enum>(static_cast<int>(utility::jwday(jd.jd())));
	}

} // end-of-namespace khronos
