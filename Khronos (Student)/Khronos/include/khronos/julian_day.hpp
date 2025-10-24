#pragma once
/*	julian_day.hpp

	Khronos library Julian Day class declaration.

=============================================================
Revision History
-------------------------------------------------------------

Version 2021.09.25
	Repaced relational operators with <=>

Version 2016.09.30
	Added: Phase List

Version 2015.09.21
	Initial Release


=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han, Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/

#include <cmath>
#include <string>
#include <ostream>
#include <compare>

#include <khronos/def.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/utility.hpp>

namespace khronos {

	using jd_t = double;

	// Helper types for arithmetic operations
	namespace detail {
		struct packaged_year_real {
			double value;
			packaged_year_real(double v) : value(v) {}
		};

		struct packaged_month_integer {
			int value;
			packaged_month_integer(int v) : value(v) {}
		};

		struct packaged_week {
			double value;
			packaged_week(double v) : value(v) {}
		};

		struct packaged_day {
			double value;
			packaged_day(double v) : value(v) {}
		};

		struct packaged_hour {
			double value;
			packaged_hour(double v) : value(v) {}
		};

		struct packaged_minute {
			double value;
			packaged_minute(double v) : value(v) {}
		};

		struct packaged_second {
			double value;
			packaged_second(double v) : value(v) {}
		};
	}

	// UDL operators for time periods
	constexpr detail::packaged_year_real operator ""_years(long double v) {
		return detail::packaged_year_real(static_cast<double>(v));
	}

	constexpr detail::packaged_year_real operator ""_year(long double v) {
		return detail::packaged_year_real(static_cast<double>(v));
	}

	constexpr detail::packaged_month_integer operator ""_months(unsigned long long v) {
		return detail::packaged_month_integer(static_cast<int>(v));
	}

	constexpr detail::packaged_month_integer operator ""_month(unsigned long long v) {
		return detail::packaged_month_integer(static_cast<int>(v));
	}

	constexpr detail::packaged_week operator ""_weeks(long double v) {
		return detail::packaged_week(static_cast<double>(v));
	}

	constexpr detail::packaged_week operator ""_week(long double v) {
		return detail::packaged_week(static_cast<double>(v));
	}

	constexpr detail::packaged_day operator ""_days(long double v) {
		return detail::packaged_day(static_cast<double>(v));
	}

	constexpr detail::packaged_day operator ""_day(long double v) {
		return detail::packaged_day(static_cast<double>(v));
	}

	constexpr detail::packaged_hour operator ""_hours(long double v) {
		return detail::packaged_hour(static_cast<double>(v));
	}

	constexpr detail::packaged_hour operator ""_hour(long double v) {
		return detail::packaged_hour(static_cast<double>(v));
	}

	constexpr detail::packaged_minute operator ""_minutes(long double v) {
		return detail::packaged_minute(static_cast<double>(v));
	}

	constexpr detail::packaged_minute operator ""_minute(long double v) {
		return detail::packaged_minute(static_cast<double>(v));
	}

	constexpr detail::packaged_second operator ""_seconds(long double v) {
		return detail::packaged_second(static_cast<double>(v));
	}

	constexpr detail::packaged_second operator ""_second(long double v) {
		return detail::packaged_second(static_cast<double>(v));
	}

	// Jd class - represents Julian Day Number with fractional part for time
	class Jd {
		jd_t jd_;

	public:
		// Constructors
		Jd();
		Jd(now_t now);
		explicit Jd(jd_t jd) : jd_(jd) {}

		// Accessor
		jd_t jd() const { return jd_; }

		// Conversion
		operator jd_t() const { return jd_; }

		// Arithmetic operations with time units
		Jd& operator+=(detail::packaged_second const& sec) {
			jd_ += sec.value / SECONDS_PER_DAY;
			return *this;
		}

		Jd& operator+=(detail::packaged_minute const& min) {
			jd_ += min.value / (24.0 * 60.0);
			return *this;
		}

		Jd& operator+=(detail::packaged_hour const& hr) {
			jd_ += hr.value / 24.0;
			return *this;
		}

		Jd& operator+=(detail::packaged_day const& d) {
			jd_ += d.value;
			return *this;
		}

		Jd& operator+=(detail::packaged_week const& w) {
			jd_ += w.value * 7.0;
			return *this;
		}

		Jd& operator+=(detail::packaged_year_real const& y) {
			jd_ += y.value * EARTH_ORBITAL_PERIOD_DAYS;
			return *this;
		}

		// Subtraction operators
		Jd& operator-=(detail::packaged_second const& sec) {
			jd_ -= sec.value / SECONDS_PER_DAY;
			return *this;
		}

		Jd& operator-=(detail::packaged_minute const& min) {
			jd_ -= min.value / (24.0 * 60.0);
			return *this;
		}

		Jd& operator-=(detail::packaged_hour const& hr) {
			jd_ -= hr.value / 24.0;
			return *this;
		}

		Jd& operator-=(detail::packaged_day const& d) {
			jd_ -= d.value;
			return *this;
		}

		Jd& operator-=(detail::packaged_week const& w) {
			jd_ -= w.value * 7.0;
			return *this;
		}

		Jd& operator-=(detail::packaged_year_real const& y) {
			jd_ -= y.value * EARTH_ORBITAL_PERIOD_DAYS;
			return *this;
		}

		// Addition operators returning new Jd
		Jd operator+(detail::packaged_second const& sec) const {
			return Jd(jd_ + sec.value / SECONDS_PER_DAY);
		}

		Jd operator+(detail::packaged_minute const& min) const {
			return Jd(jd_ + min.value / (24.0 * 60.0));
		}

		Jd operator+(detail::packaged_hour const& hr) const {
			return Jd(jd_ + hr.value / 24.0);
		}

		Jd operator+(detail::packaged_day const& d) const {
			return Jd(jd_ + d.value);
		}

		Jd operator+(detail::packaged_week const& w) const {
			return Jd(jd_ + w.value * 7.0);
		}

		Jd operator+(detail::packaged_year_real const& y) const {
			return Jd(jd_ + y.value * EARTH_ORBITAL_PERIOD_DAYS);
		}

		// Subtraction operators returning new Jd
		Jd operator-(detail::packaged_second const& sec) const {
			return Jd(jd_ - sec.value / SECONDS_PER_DAY);
		}

		Jd operator-(detail::packaged_minute const& min) const {
			return Jd(jd_ - min.value / (24.0 * 60.0));
		}

		Jd operator-(detail::packaged_hour const& hr) const {
			return Jd(jd_ - hr.value / 24.0);
		}

		Jd operator-(detail::packaged_day const& d) const {
			return Jd(jd_ - d.value);
		}

		Jd operator-(detail::packaged_week const& w) const {
			return Jd(jd_ - w.value * 7.0);
		}

		Jd operator-(detail::packaged_year_real const& y) const {
			return Jd(jd_ - y.value * EARTH_ORBITAL_PERIOD_DAYS);
		}
	};

	// Relational operators
	constexpr bool operator==(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() == rhs.jd();
	}

	constexpr auto operator<=>(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() <=> rhs.jd();
	}

	constexpr bool operator!=(Jd const& lhs, Jd const& rhs) {
		return !(lhs == rhs);
	}

	constexpr bool operator<(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() < rhs.jd();
	}

	constexpr bool operator<=(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() <= rhs.jd();
	}

	constexpr bool operator>(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() > rhs.jd();
	}

	constexpr bool operator>=(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() >= rhs.jd();
	}

	// Difference operator
	constexpr jd_t operator-(Jd const& lhs, Jd const& rhs) {
		return lhs.jd() - rhs.jd();
	}

	// Comparison with jd_t
	constexpr bool operator==(Jd const& lhs, jd_t rhs) {
		return lhs.jd() == rhs;
	}

	constexpr bool operator==(jd_t lhs, Jd const& rhs) {
		return lhs == rhs.jd();
	}

} // end-of-namespace khronos
