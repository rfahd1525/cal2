#pragma once

/*
khronos\timeofday.hpp
(c) Garth Santor
Created: 2015-09-22
Last Updated: 2015-09-22

Khronos library 'time-of-day' declarations.
*/


#include <khronos/def.hpp>


namespace khronos {
	// LITERALS
	/* UDL - converts an 'AM' hour to 'military' hour. */
	constexpr hour_t operator ""_am(unsigned long long hours) {
		return hours == 12 ? 0 : static_cast<hour_t>(hours);
	}

	/* UDL - converts an 'PM' hour to 'military' hour. */
	constexpr hour_t operator ""_pm(unsigned long long hours) {
		return hours == 12 ? 12 : static_cast<hour_t>(hours) + 12;
	}

	/* UDL - converts a CE year to astronomical year. */
	constexpr year_t operator ""_CE(unsigned long long year) {
		return static_cast<year_t>(year);
	}

	/* UDL - converts a BCE year to astronomical year. */
	constexpr year_t operator ""_BCE(unsigned long long year) {
		return 1 - static_cast<year_t>(year);
	}

	/* Seconds per day. */
	constexpr second_t SECONDS_PER_DAY = 24 * 60 * 60;

	/* H:M:S.S to time-of-day [0.0..1.0) */
	constexpr tod_t tod(hour_t hours, minute_t minutes, second_t seconds) {
		return (hours * 3600.0 + minutes * 60.0 + seconds) / SECONDS_PER_DAY;
	}

	/**	Convert Time-of-day (TOD) to hours, minutes, seconds.  Civil days run from midnight to midnight.
		@param tod [in] a time-of-day [0..1)
		@param hours [out] 0-23
		@param minutes [out] 0-59
		@param seconds [out] [0.0..60.0)
		*/
	inline void jd_to_hms(tod_t tod, hour_t& hours, minute_t& minutes, second_t& seconds) {
		second_t totalSeconds = tod * SECONDS_PER_DAY;
		hours = static_cast<hour_t>(totalSeconds / 3600.0);
		totalSeconds -= hours * 3600.0;
		minutes = static_cast<minute_t>(totalSeconds / 60.0);
		seconds = totalSeconds - minutes * 60.0;
	}

} // end-of-namespace khronos
