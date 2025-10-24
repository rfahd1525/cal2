/**	@file Islamic.cpp
	@author Garth Santor
	@date 2013-09-20

	Islamic calendar class implementation.
	*/

#include <khronos/islamic_calendar.hpp>
#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>

namespace khronos {

	// Default constructor - current date and time
	Islamic::Islamic() {
		time_t now = time(nullptr);
		struct tm local;
		localtime_s(&local, &now);

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
	Islamic::Islamic(now_t nowType) {
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
	Islamic::Islamic(year_t year, month_t month, day_t day) {
		jd_ = islamic_to_jd(year, month, day);
	}

	// Constructor from year, month, day, hour, minute, second
	Islamic::Islamic(year_t year, month_t month, day_t day,
	                  hour_t hour, minute_t minute, second_t second) {
		jd_ = islamic_to_jd(year, month, day, hour, minute, second);
	}

	// Constructor from Jd
	Islamic::Islamic(Jd const& jd) : jd_(jd.jd()) {}

	// Accessors
	year_t Islamic::year() const {
		year_t y; month_t m; day_t d;
		jd_to_islamic(jd_, y, m, d);
		return y;
	}

	month_t Islamic::month() const {
		year_t y; month_t m; day_t d;
		jd_to_islamic(jd_, y, m, d);
		return m;
	}

	day_t Islamic::day() const {
		year_t y; month_t m; day_t d;
		jd_to_islamic(jd_, y, m, d);
		return d;
	}

	hour_t Islamic::hour() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_islamic(jd_, y, m, d, h, min, s);
		return h;
	}

	minute_t Islamic::minute() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_islamic(jd_, y, m, d, h, min, s);
		return min;
	}

	second_t Islamic::second() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_islamic(jd_, y, m, d, h, min, s);
		return s;
	}

	// Conversion operators
	Islamic::operator Jd() const {
		return Jd(jd_);
	}

	Islamic& Islamic::operator=(Jd const& jd) {
		jd_ = jd.jd();
		return *this;
	}

	// Arithmetic operators - days and weeks
	Islamic Islamic::operator+(days const& d) const {
		return Islamic(Jd(jd_ + d.value()));
	}

	Islamic Islamic::operator-(days const& d) const {
		return Islamic(Jd(jd_ - d.value()));
	}

	Islamic Islamic::operator+(weeks const& w) const {
		return Islamic(Jd(jd_ + w.value() * 7.0));
	}

	Islamic Islamic::operator-(weeks const& w) const {
		return Islamic(Jd(jd_ - w.value() * 7.0));
	}

	// Arithmetic operators - months
	Islamic Islamic::operator+(months const& m) const {
		year_t y; month_t mon; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_islamic(jd_, y, mon, d, h, min, s);

		int totalMonths = mon + m.value();
		y += (totalMonths - 1) / 12;
		mon = ((totalMonths - 1) % 12) + 1;

		day_t maxDay = islamic_days_in_month(mon, is_islamic_leapyear(y));
		if (d > maxDay) {
			d = maxDay;
		}

		return Islamic(y, mon, d, h, min, s);
	}

	Islamic Islamic::operator-(months const& m) const {
		return *this + months(-m.value());
	}

	// Arithmetic operators - years
	Islamic Islamic::operator+(years const& y) const {
		year_t yr; month_t mon; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_islamic(jd_, yr, mon, d, h, min, s);

		yr += static_cast<year_t>(y.value());

		// Handle day overflow
		day_t maxDay = islamic_days_in_month(mon, is_islamic_leapyear(yr));
		if (d > maxDay) {
			d = maxDay;
		}

		return Islamic(yr, mon, d, h, min, s);
	}

	Islamic Islamic::operator-(years const& y) const {
		return *this + years(-y.value());
	}

	// String conversion
	std::string Islamic::to_string() const {
		year_t y; month_t m; day_t d;
		hour_t h; minute_t min; second_t s;
		jd_to_islamic(jd_, y, m, d, h, min, s);

		day_of_week_enum dow = day_of_week(*this);

		std::ostringstream oss;
		oss << civil::day_name(dow) << ", "
		    << islamic_month_name(m) << " " << d << " "
		    << y << " AH";

		hour_t hour12 = h % 12;
		if (hour12 == 0) hour12 = 12;

		oss << ", " << hour12 << ":"
		    << std::setfill('0') << std::setw(2) << min << ":"
		    << std::setw(2) << std::fixed << std::setprecision(0) << s
		    << (h < 12 ? " am" : " pm");

		return oss.str();
	}

	// Stream operator
	std::ostream& operator<<(std::ostream& os, Islamic const& i) {
		return os << i.to_string();
	}

	// Day of week function
	day_of_week_enum day_of_week(Islamic const& i) {
		return day_of_week(Jd(i));
	}

} // end-of-namespace khronos
