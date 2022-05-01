enum LoggerInstanceMask
{
	SERVER,
	CLIENT,
	ALL = SERVER | CLIENT
}

class LoggerBase
{
	static const string BASE_DIRECTORY = "$profile:\\";
	protected static LoggerManager m_LoggerManager;
	
	protected string m_FileName;
	
	void LoggerBase()
	{
		m_LoggerManager = LoggerManager.GetInstance();
		
		string folder_name = BASE_DIRECTORY + GetFolderName();
		if (!MakeDirectory(folder_name)) {
			Error("LoggerBase failed to make directory, exiting");
			return;
		}
		
		string file_name = GetFileFormat();
		file_name.Replace("%t%", Type().ToString());
		
		int y, m, d, hh, mm, ss;
		GetYearMonthDay(y, m, d);
		file_name.Replace("%y%", y.ToString());
		file_name.Replace("%m%", m.ToString());
		file_name.Replace("%d%", d.ToString());
		
		GetHourMinuteSecond(hh, mm, ss);		
		file_name.Replace("%hh%", hh.ToString());
		file_name.Replace("%mm%", mm.ToString());
		file_name.Replace("%ss%", ss.ToString());
		
		m_FileName = folder_name + "\\" + file_name + GetFileExtension();
				
		// Create the file
		CloseFile(OpenFile(m_FileName, FileMode.WRITE));
	}
			
	static void DoLog(typename logger_type, string log)
	{		
		LoggerBase logger_base = LoggerManager.GetInstance().GetLogger(logger_type);
		if (!logger_base) {
			Error("No logger found with type " + logger_type);
			return;
		}
		
		int log_mask = logger_base.GetLogMask();
		if ((log_mask & LoggerInstanceMask.SERVER) != LoggerInstanceMask.SERVER && GetGame().IsServer()) {
			return;
		}
		
		if ((log_mask & LoggerInstanceMask.CLIENT) != LoggerInstanceMask.CLIENT && GetGame().IsClient()) {
			return;
		}

		FileHandle handle = OpenFile(logger_base.GetFileName(), FileMode.APPEND);
		FPrintln(handle, log);
		CloseFile(handle);
	}
		
	// override this
	static void Log(string log);

	string GetFileName()
	{
		return m_FileName;
	}
	
	string GetFolderName()
	{
		return Type().ToString();
	}
	
	string GetFileExtension()
	{
		return ".log";
	}
	
	int GetLogMask()
	{
		return LoggerInstanceMask.SERVER;
	}
	
	/*
		%t% = logger typename
		%y% = year
		%m% = month
		%d% = day
		%hh% = hour
		%mm% = minute
		%ss% = second
	*/
	string GetFileFormat()
	{
		return "%t%_%y%-%m%-%d%_%hh%-%mm%-%ss%";
	}
}