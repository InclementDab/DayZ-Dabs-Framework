typedef int TimeSpan;
class TimeSpan: int
{
	static const TimeSpan DAY = 86400;
	static const TimeSpan HOUR = 3600;
	static const TimeSpan MINUTE = 60;
	
	TimeSpan GetDays()
	{
		return (value / (24 * HOUR));
	}
	
	TimeSpan GetHours()
	{
		return (value % (24 * HOUR)) / HOUR;
	}
	
	TimeSpan GetMinutes()
	{
		return (value % HOUR) / MINUTE;
	}
	
	TimeSpan GetSeconds()
	{
		return (value % MINUTE);
	}
		
	string Format(bool with_zero = true)
	{
		/*
		if (with_zero) {
			return string.Format("%1#STR_time_unit_abbrev_day_0 %2#STR_time_unit_abbrev_hour_0 %3#STR_time_unit_abbrev_minute_0 %4#STR_time_unit_abbrev_second_0", GetDays(), GetHours(), GetMinutes(), GetSeconds());
		}*/
		
		string message;
		TimeSpan days = (value / DAY);
		if (days > 0) {
			message += string.Format("%1%2 ", days, Widget.TranslateString("#STR_time_unit_abbrev_day_0"));
		}
		
		TimeSpan hours = (value % DAY) / HOUR;
		if (hours > 0)
			message += string.Format("%1%2 ", hours, Widget.TranslateString("#STR_time_unit_abbrev_hour_0"));
		
		TimeSpan minutes = (value % HOUR) / MINUTE;
		if (minutes > 0)
			message += string.Format("%1%2 ", minutes, Widget.TranslateString("#STR_time_unit_abbrev_minute_0"));

		message += string.Format("%1%2", (value % MINUTE), Widget.TranslateString("#STR_time_unit_abbrev_second_0"));
		
		return message;
	}
	
	static TimeSpan FromMinutes(float minutes)
	{
		return minutes * (float)MINUTE;
	}
	
	static TimeSpan FromHours(float hours)
	{
		return hours * (float)HOUR;
	}
	
	static TimeSpan FromDays(float days)
	{
		return days * (float)DAY;
	}
	
	private void TimeSpan();
	private void ~TimeSpan();
}