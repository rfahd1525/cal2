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

#include <khronos/def.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/utility.hpp>



namespace khronos {

	// Forward declarations
	class days;
	class weeks;
	class years;

	/**
	 * Julian Day class - stores time as a Julian Day Number with fractional part for time.
	 * JD 0.0 represents noon on January 1, 4713 BCE (proleptic Julian calendar).
	 * The fractional part represents time, where .0 is noon and .5 is midnight.
	 */
	class Jd {
		jd_t jd_;

	public:
		// Constructors
		Jd();  // default to current time
		explicit Jd(now_t nowType);  // now with or without time
		explicit Jd(jd_t jd) : jd_(jd) {}

		// Accessor
		jd_t jd() const { return jd_; }

		// Arithmetic operators
		Jd operator+(days const& d) const;
		Jd operator-(days const& d) const;
		Jd operator+(weeks const& w) const;
		Jd operator-(weeks const& w) const;
		Jd operator+(years const& y) const;
		Jd operator-(years const& y) const;

		jd_t operator-(Jd const& rhs) const { return jd_ - rhs.jd_; }

		// Relational operators
		auto operator<=>(Jd const& rhs) const { return jd_ <=> rhs.jd_; }
		bool operator==(Jd const& rhs) const { return jd_ == rhs.jd_; }
		bool operator!=(Jd const& rhs) const { return jd_ != rhs.jd_; }
		bool operator<(Jd const& rhs) const { return jd_ < rhs.jd_; }
		bool operator<=(Jd const& rhs) const { return jd_ <= rhs.jd_; }
		bool operator>(Jd const& rhs) const { return jd_ > rhs.jd_; }
		bool operator>=(Jd const& rhs) const { return jd_ >= rhs.jd_; }

		// Comparison with raw jd_t
		bool operator==(jd_t rhs) const { return jd_ == rhs; }

		// String conversion
		std::string to_string() const;
	};

	// Stream operator
	std::ostream& operator<<(std::ostream& os, Jd const& jd);

	// Day of week function
	day_of_week_enum day_of_week(Jd const& jd);

} // end-of-namespace khronos