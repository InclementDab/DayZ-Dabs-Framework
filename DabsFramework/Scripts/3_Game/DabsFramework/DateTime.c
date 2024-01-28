class DateTime: int
{		
	static const int MINUTES_PER_HOUR = 60;
	static const int HOURS_PER_DAY = 24;
	
	static const DateTime EPOCH = 0;
	
	// https://en.wikipedia.org/wiki/ISO_8601
	static const string FORMAT_ISO_DATE = "yyyy-MM-dd";
	static const string FORMAT_ISO_TIME = "HH:mm:ss";
	static const string FORMAT_ISO_DATETIME = "yyyy-MM-ddTHH:mm:ss";
	
	static const string MONTH_NAME_LONG[12] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
	static const string MONTH_NAME_SHORT[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	static const string DAY_NAME_LONG[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	static const string DAY_NAME_SHORT[7] = { "Mon", "Tues", "Wed", "Thur", "Fri", "Sat", "Sun" };
	static const string MERIDIEM_LONG[2] = { "AM", "PM" };
	static const string MERIDIEM_SHORT[2] = { "A", "P" };
	
	private void DateTime();
	private void ~DateTime();
		
	/*
		Supported formatting:
			
		d: Represents the day of the month as a number from 1 through 31.
		dd: Represents the day of the month as a number from 01 through 31.
		ddd: Represents the abbreviated name of the day (Mon, Tues, Wed, etc).
		dddd: Represents the full name of the day (Monday, Tuesday, etc).
		h: 12-hour clock hour (e.g. 4).
		hh: 12-hour clock, with a leading 0 (e.g. 06)
		H: 24-hour clock hour (e.g. 15)
		HH: 24-hour clock hour, with a leading 0 (e.g. 22)
		m: Minutes
		mm: Minutes with a leading zero
		M: Month number(eg.3)
		MM: Month number with leading zero(eg.04)
		MMM: Abbreviated Month Name (e.g. Dec)
		MMMM: Full month name (e.g. December)
		s: Seconds
		ss: Seconds with leading zero
		t: Abbreviated AM / PM (e.g. A or P)
		tt: AM / PM (e.g. AM or PM
		y: Year, no leading zero (e.g. 2015 would be 15)
		yy: Year, leading zero (e.g. 2015 would be 015)
		yyy: Year, (e.g. 2015)
		yyyy: Year, (e.g. 2015)
	*/
	
	string ToString(string format)
	{
		int year, month, day, hour, minute, second;
		DateTime.ToDate(value, year, month, day, hour, minute, second);
		
		int day_of_week = DateTime.GetDayOfWeek(year, month, day);
		format.Replace("dddd", DAY_NAME_LONG[day_of_week]);
		format.Replace("ddd", DAY_NAME_SHORT[day_of_week]);
		format.Replace("dd", day.ToStringLen(2));
		format.Replace("d", day.ToString());
		
		int twelve_hour_format = Ternary<int>.If(hour > 12, hour % 12, hour);
		format.Replace("hh", twelve_hour_format.ToStringLen(2));
		format.Replace("h", twelve_hour_format.ToString());
		format.Replace("HH", hour.ToStringLen(2));
		format.Replace("H", hour.ToString());
		
		format.Replace("mm", minute.ToStringLen(2));
		format.Replace("m", minute.ToString());
		
		format.Replace("MMMM", MONTH_NAME_LONG[month - 1]);
		format.Replace("MMM", MONTH_NAME_SHORT[month - 1]);
		format.Replace("MM", month.ToStringLen(2));
		format.Replace("M", month.ToString());
		
		format.Replace("ss", second.ToStringLen(2));
		format.Replace("s", second.ToString());
		
		format.Replace("tt", MERIDIEM_LONG[hour / 12]);
		format.Replace("t", MERIDIEM_SHORT[hour / 12]);
		
		format.Replace("yyyy", year.ToStringLen(4));
		format.Replace("yyy", year.ToString());
		format.Replace("yy", year.ToString().Substring(1, 3));
		format.Replace("y", year.ToString().Substring(2, 2));
		
		return format;
	}
	
	void GetDate(out int year, out int month, out int day, out int hour, out int minute, out int second)
	{
		DateTime.ToDate(value, year, month, day, hour, minute, second);
	}
	
	static DateTime Now(bool utc = true)
	{
		int year, month, day, hour, minute, second;
		if (utc) {
			GetHourMinuteSecondUTC(hour, minute, second);
			GetYearMonthDayUTC(year, month, day);
		} else {
			GetHourMinuteSecond(hour, minute, second);
			GetYearMonthDay(year, month, day);
		}
		
		return DateTime.Create(year, month, day, hour, minute, second);
	}
		
	static DateTime Create(int year, int month, int day, int hour, int minute, int second)
	{
		DateTime timestamp = EPOCH;
		if (year < 1970) {
			return timestamp;
		}
		
		//@ Day 1 of the month has no time passed
		//		same reason why our month index starts at 1 not 0
		int total_days = day - 1; 
		for (int epoch_year = 1970; epoch_year < year; epoch_year++) {
			total_days += DateTime.DaysInYear(epoch_year);
		}
		
		for (int epoch_month = 1; epoch_month < month; epoch_month++) {
			total_days += DateTime.DaysInMonth(epoch_month, DateTime.IsLeapYear(year));
		}
		
		timestamp += total_days * TimeSpan.DAY;
		timestamp += hour * TimeSpan.HOUR;
		timestamp += minute * TimeSpan.MINUTE;
		timestamp += second;
		
		return timestamp;
	}
	
	static int DaysInMonth(int month, bool leap_year = false)
	{
		static const int DAYS_IN_MONTH[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		switch (month) {
			case 1: // January
			case 3: // March
			case 5: // May
			case 7: // July
			case 8: // August
			case 10: // October
			case 12: // December
				return 31;
			case 4: // April
			case 6: // June
			case 9: // September
			case 11: // November
				return 30;
			
			case 2: // Februray
				return Ternary<int>.If(leap_year, 29, 28);
		}
		
		Error("Invalid Month " + month);
		return 0;
	}
	
	static int DaysInYear(int year)
	{
		return Ternary<int>.If(DateTime.IsLeapYear(year), 366, 365);
	}
		
	static bool IsLeapYear(int year)
	{
		if (year % 400 == 0) {
			return true;
		}

		if (year % 100 == 0) {
			return false;
		}

		if (year % 4 == 0) {
			return true;
		}

		return false;
	}
	
	// https://en.wikipedia.org/wiki/Zeller%27s_congruence
	static int GetDayOfWeek(int year, int month, int day)
	{
		if (month < 3) {
			month += 12;
			year -= 1;
		}
		
		return (day + 13 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
	}
	
	static void ToDate(DateTime date_time, out int year, out int month, out int day, out int hour, out int minute, out int second)
	{		
		int total_minutes = date_time / TimeSpan.MINUTE;
		second = date_time % TimeSpan.MINUTE;
		
		int total_hours = total_minutes / MINUTES_PER_HOUR;
		minute = total_minutes % MINUTES_PER_HOUR;
		
		day = total_hours / HOURS_PER_DAY;
		hour = total_hours % HOURS_PER_DAY;
		
		year = 1970;
		month = 1;
		
		// Checking ahead on the next month / year
		int days_in_month = DateTime.DaysInMonth(month, DateTime.IsLeapYear(year));
		while (day > days_in_month) {			
			day -= days_in_month;
			month++;
			if (month > 12) {
				month = 1;
				year++;
			}
			
			days_in_month = DateTime.DaysInMonth(month, DateTime.IsLeapYear(year));
		}
		
		// need to advance this by 1, since days start at 1 for whatever reason
		day++;
	}
	
	//@ Just so you see how the system works
	static TimeSpan Subtract(DateTime t1, DateTime t2)
	{
		return t1 - t2;
	}
}

typedef int DateTime;