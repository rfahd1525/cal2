/**	@file Julian.cpp
	@author Garth Santor
	@date 2013-09-19

	Julian calendar class implementation.
	*/

#include <khronos/julian_calendar.hpp>
#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>

namespace khronos {

	// Default constructor - current date and time
	Julian::Julian() {
		time_t now = time(nullptr);
		struct tm local;
		localtime_s(&local, &now);

		// Convert current Gregorian date to JD, then to Julian calendar
		jd_t jdNow = gregorian_to_jd(
			year_t(local.tm_year) + 1900,
			month_t(local.tm_mon) + 1,
			day_t(local.tm_mday),
			hour_t(local.tm_hour),
			minute_t(local.tm_min),
			second_t(local.tm_sec)
		);
		jd_ = jdNow;
	}

	// Constructor with now_t parameter
	Julian::Julian(now_t nowType) {
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
	Julian::Julian(year_t year, month_t month, day_t day) {
		jd_ = julian_to_jd(year, month, day);
	}

	// Constructor from year, month, day, hour, minute, second
	Julian::Julian(year_t year, month_t month, day_t day,
	                hour_t hour, minute_t minute, second_t second) {
		jd_ = julian_to_jd(year, month, day, hour, minute, second);
	}

	// Constructor from Jd
	Julian::Julian(Jd const& jd) : jd_(jd.jd()) {}

	// Accessors
	year_t Julian::year() const {
		year_t y; month_t m; day_t d;
		jd_to_julian(jd_, y, m, d);
		return y;
	}

	month_t Julian::month() const {
		year_t y; month_t m; day_t d;
		jd_to_julian(jd_, y, m, d);
		return m;
	}

	day_t Julian::day() const {
		year_t y; month_t m; day_t d;
		jd_to_julian(jd_, y, m, d);
		return d;
	}

	hour_t Julian::hour() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_julian(jd_, y, m, d, h, min, s);
		return h;
	}

	minute_t Julian::minute() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_julian(jd_, y, m, d, h, min, s);
		return min;
	}

	second_t Julian::second() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_julian(jd_, y, m, d, h, min, s);
		return s;
	}

	// Conversion operators
	Julian::operator Jd() const {
		return Jd(jd_);
	}

	Julian& Julian::operator=(Jd const& jd) {
		jd_ = jd.jd();
		return *this;
	}

	// Arithmetic operators - days and weeks
	Julian Julian::operator+(days const& d) const {
		return Julian(Jd(jd_ + d.value()));
	}

	Julian Julian::operator-(days const& d) const {
		return Julian(Jd(jd_ - d.value()));
	}

	Julian Julian::operator+(weeks const& w) const {
		return Julian(Jd(jd_ + w.value() * 7.0));
	}

	Julian Julian::operator-(weeks const& w) const {
		return Julian(Jd(jd_ - w.value() * 7.0));
	}

	// Arithmetic operators - months
	Julian Julian::operator+(months const& m) const {
		year_t y; month_t mon; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_julian(jd_, y, mon, d, h, min, s);

		int totalMonths = mon + m.value();
		y += (totalMonths - 1) / 12;
		mon = ((totalMonths - 1) % 12) + 1;

		// Adjust day if needed
		day_t maxDay = julian_days_in_month(mon, is_julian_leapyear(y));
		if (d > maxDay) {
			d = maxDay;
		}

		return Julian(y, mon, d, h, min, s);
	}

	Julian Julian::operator-(months const& m) const {
		return *this + months(-m.value());
	}

	// Arithmetic operators - years
	Julian Julian::operator+(years const& y) const {
		year_t yr; month_t mon; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_julian(jd_, yr, mon, d, h, min, s);

		yr += static_cast<year_t>(y.value());

		// Handle Feb 29 on leap to non-leap
		if (mon == February && d == 29 && !is_julian_leapyear(yr)) {
			d = 28;
		}

		return Julian(yr, mon, d, h, min, s);
	}

	Julian Julian::operator-(years const& y) const {
		return *this + years(-y.value());
	}

	// String conversion
	std::string Julian::to_string() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_julian(jd_, y, m, d, h, min, s);

		// Get day of week
		day_of_week_enum dow = day_of_week(*this);

		std::ostringstream oss;
		oss << civil::day_name(dow) << ", "
		    << julian_month_name(m) << " " << d << " ";

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
	std::ostream& operator<<(std::ostream& os, Julian const& j) {
		return os << j.to_string();
	}

	// Day of week function
	day_of_week_enum day_of_week(Julian const& j) {
		return day_of_week(Jd(j));
	}

} // end-of-namespace khronos
