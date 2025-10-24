/**	@file	gregorian.cpp
	@author	Garth Santor
	@date	2013-09-19

	Gregorian calendar class implementation.
	*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/julian_day.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>

namespace khronos {

	// Default constructor - current date and time
	Gregorian::Gregorian() {
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
	Gregorian::Gregorian(now_t nowType) {
		time_t now = time(nullptr);
		struct tm local;
		localtime_s(&local, &now);

		if (nowType == NOTIMEOFDAY) {
			jd_ = gregorian_to_jd(
				year_t(local.tm_year) + 1900,
				month_t(local.tm_mon) + 1,
				day_t(local.tm_mday)
			);
		} else {
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

	// Constructor from year, month, day
	Gregorian::Gregorian(year_t year, month_t month, day_t day) {
		jd_ = gregorian_to_jd(year, month, day);
	}

	// Constructor from year, month, day, hour, minute, second
	Gregorian::Gregorian(year_t year, month_t month, day_t day,
	                      hour_t hour, minute_t minute, second_t second) {
		jd_ = gregorian_to_jd(year, month, day, hour, minute, second);
	}

	// Constructor from Jd
	Gregorian::Gregorian(Jd const& jd) : jd_(jd.jd()) {}

	// Accessors
	year_t Gregorian::year() const {
		year_t y; month_t m; day_t d;
		jd_to_gregorian(jd_, y, m, d);
		return y;
	}

	month_t Gregorian::month() const {
		year_t y; month_t m; day_t d;
		jd_to_gregorian(jd_, y, m, d);
		return m;
	}

	day_t Gregorian::day() const {
		year_t y; month_t m; day_t d;
		jd_to_gregorian(jd_, y, m, d);
		return d;
	}

	hour_t Gregorian::hour() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_gregorian(jd_, y, m, d, h, min, s);
		return h;
	}

	minute_t Gregorian::minute() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_gregorian(jd_, y, m, d, h, min, s);
		return min;
	}

	second_t Gregorian::second() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_gregorian(jd_, y, m, d, h, min, s);
		return s;
	}

	// Conversion operators
	Gregorian::operator Jd() const {
		return Jd(jd_);
	}

	Gregorian& Gregorian::operator=(Jd const& jd) {
		jd_ = jd.jd();
		return *this;
	}

	// Arithmetic operators - days and weeks
	Gregorian Gregorian::operator+(days const& d) const {
		return Gregorian(Jd(jd_ + d.value()));
	}

	Gregorian Gregorian::operator-(days const& d) const {
		return Gregorian(Jd(jd_ - d.value()));
	}

	Gregorian Gregorian::operator+(weeks const& w) const {
		return Gregorian(Jd(jd_ + w.value() * 7.0));
	}

	Gregorian Gregorian::operator-(weeks const& w) const {
		return Gregorian(Jd(jd_ - w.value() * 7.0));
	}

	// Arithmetic operators - months
	Gregorian Gregorian::operator+(months const& m) const {
		year_t y; month_t mon; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_gregorian(jd_, y, mon, d, h, min, s);

		int totalMonths = mon + m.value();
		y += (totalMonths - 1) / 12;
		mon = ((totalMonths - 1) % 12) + 1;

		// Adjust day if it exceeds the days in the target month
		day_t maxDay = gregorian_days_in_month(mon, is_gregorian_leapyear(y));
		if (d > maxDay) {
			d = maxDay;
		}

		return Gregorian(y, mon, d, h, min, s);
	}

	Gregorian Gregorian::operator-(months const& m) const {
		return *this + months(-m.value());
	}

	// Arithmetic operators - years
	Gregorian Gregorian::operator+(years const& y) const {
		year_t yr; month_t mon; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_gregorian(jd_, yr, mon, d, h, min, s);

		yr += static_cast<year_t>(y.value());

		// Handle February 29 on leap year to non-leap year
		if (mon == February && d == 29 && !is_gregorian_leapyear(yr)) {
			d = 28;
		}

		return Gregorian(yr, mon, d, h, min, s);
	}

	Gregorian Gregorian::operator-(years const& y) const {
		return *this + years(-y.value());
	}

	// String conversion
	std::string Gregorian::to_string() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_gregorian(jd_, y, m, d, h, min, s);

		// Get day of week
		day_of_week_enum dow = day_of_week(*this);

		// Format: "Friday, September 27 2013 CE, 11:05:03 am"
		std::ostringstream oss;
		oss << civil::day_name(dow) << ", "
		    << gregorian_month_name(m) << " " << d << " ";

		if (y > 0) {
			oss << y << " CE";
		} else {
			oss << (1 - y) << " BCE";
		}

		// Add time in 12-hour format
		hour_t hour12 = h % 12;
		if (hour12 == 0) hour12 = 12;

		oss << ", " << hour12 << ":"
		    << std::setfill('0') << std::setw(2) << min << ":"
		    << std::setw(2) << std::fixed << std::setprecision(0) << s
		    << (h < 12 ? " am" : " pm");

		return oss.str();
	}

	// Stream operator
	std::ostream& operator<<(std::ostream& os, Gregorian const& g) {
		return os << g.to_string();
	}

	// Day of week function
	day_of_week_enum day_of_week(Gregorian const& g) {
		return day_of_week(Jd(g));
	}

} // end-of-namespace khronos
