Project Description

Extend the Gregorian class to handle time of day as well and verify that
the class developed will pass the Unit Tests provided.

Time will be represented internally as the fractional part of a Julian
Day Number (see: [http://en.wikipedia.org/wiki/Julian\_day\_number]). You
should handle time input and output in hours (0-23), minutes (0-59), and
seconds (0-59). Remember that a Julian Day Number with a fractional part
of .0 refers to noon of that day.

Your solution should have a minimum of:

1.  Jd class.
	
	a.  Default constructor that initializes to now (accurate to a
	second).
	
	b.  A literal constructor that takes a real number.
	
	c.  All seven relational operators.
	
	i.  \<=\>
	
	ii. ==
	
	iii. !=
	
	iv. \<
	
	v.  \<=
	
	vi. \>
	
	vii. \>=
	
	d.  Methods/operators for adding/**subtracting**: seconds, minutes,
	hours, days, weeks, years (orbital).
	
	e.  Difference operator.

2.  Gregorian class.
	
	a.  Default constructor that initializes to now (accurate to a
	second).
	
	b.  Gregorian date constructor (no time part).
	
	c.  Gregorian date constructor (with time initialization as well).
	
	d.  Methods/operators for adding/**subtracting**:
	
	i.  seconds, minutes, hours, days, weeks (these might be shared
	    with Jd class).
	
	ii. +(years). Whole numbers only. The result should be the same
	    day in the new year. The exception is a starting date on
	    February 29 on a leap year and the ending date is a non-leap
	    year. In this case, you would use the largest day in that
	    month (February 28).
	
	iii. +(month) Whole numbers only. If you add a month the day
	     should be exactly the same as the original day. The
	     exception is if the day in the starting month is greater
	     than the last day in the ending month. In this case, you
	     would use the largest day in that month. (January 31, 2012
	     plus one month is February 29, 2012)
	
	e.  A 'to\_string' method that returns a string containing the
	Gregorian date and 12 hour time of day (e.g. '**Friday,
	September 27 2013 CE, 11:05:03 am'**).
	
	f.  Several supporting functions will help with the above
	functionality. They may include the following functions:
	is_gregorian_leap_year(), days_in_month(), jd_to_hms(),
	hms_to_days(), month_name().

3.  Your code should pass all the boost unit tests provided to cover the
	new operations. Your functions/classes should be written to pass the
	boost unit tests provided. If the boost unit tests fail, those
	operations will not be awarded full marks. You shall **not** modify
	the boost unit tests provided as a starting point. Passing the boost
	unit tests does not guarantee that you will earn all marks for that
	function if the function is not done properly

4.  Create a Julian class equivalent to the Gregorian class you have
	just created.

5.  Create the Islamic conversion algorithms to convert from the Jd
	class to Islamic and from Islamic to Jd. To test your algorithm, you
	must pass the ut\_Khronos\_13\_islamic\_concepts.cpp and
	ut\_Khronos\_14\_islamic\_conv.cpp file.

6.  Create the rest of the Islamic class equivalent to the Gregorian
	class that you have created. You should pass the ut\_Khronos\_15\_
	islamic \_class.cpp file

7.  Create the Hebrew conversion algorithms to convert from the Jd class
	to Hebrew and from Islamic to Jd. To test your algorithm, you must
	pass the ut\_Khronos\_16\_hebrew\_concepts.cpp and
	ut\_Khronos\_17\_hebrew\_conv.cpp file.

8.  Create the rest of the Hebrew class equivalent to the Gregorian
	class that you have created. You should pass the ut\_Khronos\_18\_
	hebrew \_class.cpp file

9.  Create the Vulcan conversion algorithms along with the rest of the
	Vulcan class equivalent to the Gregorian class that you have
	created. You should pass all three ut\_Khronos\_19 to 21 files.

10. You must set the test phases that you would like us to mark in the
	file ut\_Khronos.hpp. Test phases set to true will be graded.

Tips:

1.  Start off by setting all the test phases in ut\_Khronos.hpp to false
	except for the first one. Then as you implement the code determine
	which phases test your functionality and set them to true.

2.  Implement the project in the general order of the test phases.

3.  You ***must \[not]{.underline} edit*** any of the
	**ut\_Khronos\_dd.hpp** files except for the **ut\_Khronos.hpp** file.

# Grading Criteria

See: **INFO-3140 (2025) Project #1 v1.0.0.xlsx**
