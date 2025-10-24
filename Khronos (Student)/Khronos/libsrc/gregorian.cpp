/**	@file	gregorian.cpp
	@author	Garth Santor
	@date	2013-09-19

	Gregorian calendar class implementation.
	*/

#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace khronos {

	// Helper function to convert JD to Gregorian date
	void Gregorian::from_jd(jd_t jd) {
		jd_to_gregorian(jd, year_, month_, day_, hour_, minute_, second_);
	}

	// Default constructor - initializes to now with time
	Gregorian::Gregorian() {
		time_t now = time(nullptr);
		struct tm local_time;
		localtime_s(&local_time, &now);

		year_ = year_t(local_time.tm_year + 1900);
		month_ = month_t(local_time.tm_mon + 1);
		day_ = day_t(local_time.tm_mday);
		hour_ = hour_t(local_time.tm_hour);
		minute_ = minute_t(local_time.tm_min);
		second_ = second_t(local_time.tm_sec);
	}

	// Constructor with now_t parameter
	Gregorian::Gregorian(now_t now) {
		time_t current_time = time(nullptr);
		struct tm local_time;
		localtime_s(&local_time, &current_time);

		year_ = year_t(local_time.tm_year + 1900);
		month_ = month_t(local_time.tm_mon + 1);
		day_ = day_t(local_time.tm_mday);

		if (now == NOTIMEOFDAY) {
			hour_ = 0;
			minute_ = 0;
			second_ = 0;
		}
		else {
			hour_ = hour_t(local_time.tm_hour);
			minute_ = minute_t(local_time.tm_min);
			second_ = second_t(local_time.tm_sec);
		}
	}

	// Constructor from Julian Day
	Gregorian::Gregorian(jd_t jd) {
		from_jd(jd);
	}

	// Constructor from date only
	Gregorian::Gregorian(year_t year, month_t month, day_t day)
		: year_(year), month_(month), day_(day), hour_(0), minute_(0), second_(0) {
	}

	// Constructor from date and time
	Gregorian::Gregorian(year_t year, month_t month, day_t day,
		hour_t hour, minute_t minute, second_t second)
		: year_(year), month_(month), day_(day),
		hour_(hour), minute_(minute), second_(second) {
	}

	// Convert to Julian Day
	jd_t Gregorian::to_jd() const {
		return gregorian_to_jd(year_, month_, day_, hour_, minute_, second_);
	}

	// Assignment from Julian Day
	Gregorian& Gregorian::operator=(jd_t jd) {
		from_jd(jd);
		return *this;
	}

	// Add years (orbital period)
	Gregorian& Gregorian::operator+=(detail::packaged_year_real const& year) {
		// Handle whole years
		long long whole_years = static_cast<long long>(year.value);
		year_ += whole_years;

		// Check if Feb 29 on a leap year becomes Feb 28 on a non-leap year
		if (month_ == 2 && day_ == 29 && !is_gregorian_leapyear(year_)) {
			day_ = 28;
		}

		// Handle fractional years by converting to days
		double fractional_years = year.value - whole_years;
		if (fractional_years != 0.0) {
			jd_t jd = to_jd() + fractional_years * EARTH_ORBITAL_PERIOD_DAYS;
			from_jd(jd);
		}

		return *this;
	}

	// Add months
	Gregorian& Gregorian::operator+=(detail::packaged_month_integer const& month) {
		int total_months = month_ + month.value;

		// Calculate new year and month
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

		// Adjust day if it exceeds the number of days in the new month
		int max_day = gregorian_days_in_month(month_, is_gregorian_leapyear(year_));
		if (day_ > max_day) {
			day_ = max_day;
		}

		return *this;
	}

	// Add years (returning new object)
	Gregorian Gregorian::operator+(detail::packaged_year_real const& year) const {
		Gregorian result(*this);
		result += year;
		return result;
	}

	// Add months (returning new object)
	Gregorian Gregorian::operator+(detail::packaged_month_integer const& month) const {
		Gregorian result(*this);
		result += month;
		return result;
	}

	// Subtract months (returning new object)
	Gregorian Gregorian::operator-(detail::packaged_month_integer const& month) const {
		return *this + detail::packaged_month_integer(-month.value);
	}

	// to_string method
	std::string Gregorian::to_string() const {
		std::ostringstream oss;

		// Get day of week
		jd_t jd = to_jd();
		int dow = static_cast<int>(utility::jwday(jd));

		// Day of week name
		oss << civil::day_name(dow) << ", ";

		// Month name
		oss << gregorian_month_name(month_) << " ";

		// Day
		oss << static_cast<int>(day_) << " ";

		// Year with CE/BCE
		if (year_ > 0) {
			oss << year_ << " CE";
		}
		else {
			oss << (1 - year_) << " BCE";
		}

		// Time in 12-hour format
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
