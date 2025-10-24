/**	@file Hebrew.cpp
	@author Garth Santor
	@date 2013-10-20

	Hebrew calendar class implementation.
	*/

#include <khronos/hebrew_calendar.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace khronos {

	void Hebrew::from_jd(jd_t jd) {
		jd_to_hebrew(jd, year_, month_, day_, hour_, minute_, second_);
	}

	Hebrew::Hebrew() {
		time_t now = time(nullptr);
		struct tm local_time;
		localtime_s(&local_time, &now);

		jd_t jd = gregorian_to_jd(
			year_t(local_time.tm_year + 1900),
			month_t(local_time.tm_mon + 1),
			day_t(local_time.tm_mday),
			hour_t(local_time.tm_hour),
			minute_t(local_time.tm_min),
			second_t(local_time.tm_sec)
		);

		from_jd(jd);
	}

	Hebrew::Hebrew(now_t now) {
		time_t current_time = time(nullptr);
		struct tm local_time;
		localtime_s(&local_time, &current_time);

		jd_t jd;
		if (now == NOTIMEOFDAY) {
			jd = gregorian_to_jd(
				year_t(local_time.tm_year + 1900),
				month_t(local_time.tm_mon + 1),
				day_t(local_time.tm_mday)
			);
		}
		else {
			jd = gregorian_to_jd(
				year_t(local_time.tm_year + 1900),
				month_t(local_time.tm_mon + 1),
				day_t(local_time.tm_mday),
				hour_t(local_time.tm_hour),
				minute_t(local_time.tm_min),
				second_t(local_time.tm_sec)
			);
		}

		from_jd(jd);
	}

	Hebrew::Hebrew(jd_t jd) {
		from_jd(jd);
	}

	Hebrew::Hebrew(year_t year, month_t month, day_t day)
		: year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {
	}

	Hebrew::Hebrew(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second)
		: year_(year), month_(month), day_(day),
		hour_(hour), minute_(minute), second_(second) {
	}

	jd_t Hebrew::to_jd() const {
		return hebrew_to_jd(year_, month_, day_, hour_, minute_, second_);
	}

	Hebrew& Hebrew::operator=(jd_t jd) {
		from_jd(jd);
		return *this;
	}

	Hebrew& Hebrew::operator+=(detail::packaged_year_real const& year) {
		long long whole_years = static_cast<long long>(year.value);
		year_ += whole_years;

		double fractional_years = year.value - whole_years;
		if (fractional_years != 0.0) {
			jd_t jd = to_jd() + fractional_years * EARTH_ORBITAL_PERIOD_DAYS;
			from_jd(jd);
		}

		return *this;
	}

	Hebrew& Hebrew::operator+=(detail::packaged_month_integer const& month) {
		int total_months = month_ + month.value;
		int months_in_year = hebrew_year_months(year_);

		while (total_months > months_in_year) {
			total_months -= months_in_year;
			year_++;
			months_in_year = hebrew_year_months(year_);
		}

		while (total_months < 1) {
			year_--;
			months_in_year = hebrew_year_months(year_);
			total_months += months_in_year;
		}

		month_ = static_cast<month_t>(total_months);

		int max_day = hebrew_days_in_month(month_, year_);
		if (day_ > max_day) {
			day_ = max_day;
		}

		return *this;
	}

	Hebrew Hebrew::operator+(detail::packaged_year_real const& year) const {
		Hebrew result(*this);
		result += year;
		return result;
	}

	Hebrew Hebrew::operator+(detail::packaged_month_integer const& month) const {
		Hebrew result(*this);
		result += month;
		return result;
	}

	Hebrew Hebrew::operator-(detail::packaged_month_integer const& month) const {
		return *this + detail::packaged_month_integer(-month.value);
	}

	std::string Hebrew::to_string() const {
		std::ostringstream oss;

		jd_t jd = to_jd();
		int dow = static_cast<int>(utility::jwday(jd));

		oss << civil::day_name(dow) << ", ";
		oss << hebrew_month_name(month_, year_) << " ";
		oss << static_cast<int>(day_) << " ";
		oss << year_ << " A.M.";

		oss << ", ";
		int display_hour = hour_ % 12;
		if (display_hour == 0) display_hour = 12;

		oss << display_hour << ":"
			<< std::setfill('0') << std::setw(2) << minute_ << ":"
			<< std::setw(2) << std::setfill('0') << std::fixed << std::setprecision(0) << second_
			<< (hour_ < 12 ? " am" : " pm");

		return oss.str();
	}

} // end-of-namespace khronos
