/**	@file Islamic.cpp
	@author Garth Santor
	@date 2013-09-20

	Islamic calendar class implementation.
	*/

#include <khronos/islamic_calendar.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace khronos {

	void Islamic::from_jd(jd_t jd) {
		jd_to_islamic(jd, year_, month_, day_, hour_, minute_, second_);
	}

	Islamic::Islamic() {
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

	Islamic::Islamic(now_t now) {
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

	Islamic::Islamic(jd_t jd) {
		from_jd(jd);
	}

	Islamic::Islamic(year_t year, month_t month, day_t day)
		: year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {
	}

	Islamic::Islamic(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second)
		: year_(year), month_(month), day_(day),
		hour_(hour), minute_(minute), second_(second) {
	}

	jd_t Islamic::to_jd() const {
		return islamic_to_jd(year_, month_, day_, hour_, minute_, second_);
	}

	Islamic& Islamic::operator=(jd_t jd) {
		from_jd(jd);
		return *this;
	}

	Islamic& Islamic::operator+=(detail::packaged_year_real const& year) {
		long long whole_years = static_cast<long long>(year.value);
		year_ += whole_years;

		if (month_ == 12 && day_ == 30 && !is_islamic_leapyear(year_)) {
			day_ = 29;
		}

		double fractional_years = year.value - whole_years;
		if (fractional_years != 0.0) {
			jd_t jd = to_jd() + fractional_years * EARTH_ORBITAL_PERIOD_DAYS;
			from_jd(jd);
		}

		return *this;
	}

	Islamic& Islamic::operator+=(detail::packaged_month_integer const& month) {
		int total_months = month_ + month.value;

		int years_to_add = 0;
		if (total_months > 12) {
			years_to_add = (total_months - 1) / 12;
			total_months = ((total_months - 1) % 12) + 1;
		}
		else if (total_months < 1) {
			years_to_add = (total_months / 12) - 1;
			total_months = 12 + (total_months % 12);
		}

		year_ += years_to_add;
		month_ = static_cast<month_t>(total_months);

		int max_day = islamic_days_in_month(month_, is_islamic_leapyear(year_));
		if (day_ > max_day) {
			day_ = max_day;
		}

		return *this;
	}

	Islamic Islamic::operator+(detail::packaged_year_real const& year) const {
		Islamic result(*this);
		result += year;
		return result;
	}

	Islamic Islamic::operator+(detail::packaged_month_integer const& month) const {
		Islamic result(*this);
		result += month;
		return result;
	}

	Islamic Islamic::operator-(detail::packaged_month_integer const& month) const {
		return *this + detail::packaged_month_integer(-month.value);
	}

	std::string Islamic::to_string() const {
		std::ostringstream oss;

		jd_t jd = to_jd();
		int dow = static_cast<int>(utility::jwday(jd));

		oss << civil::day_name(dow) << ", ";
		oss << islamic_month_name(month_) << " ";
		oss << static_cast<int>(day_) << " ";
		oss << year_ << " A.H.";

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
