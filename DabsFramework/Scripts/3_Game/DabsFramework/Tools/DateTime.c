typedef int DateTime;
class DateTime: int
{		
	static const DateTime EPOCH = 0;
	
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
	
	//@ Just so you see how the system works
	static TimeSpan Subtract(DateTime t1, DateTime t2)
	{
		return t1 - t2;
	}
	
	private void DateTime();
	private void ~DateTime();
}