class LoggerBase
{
	static const string BASE_DIRECTORY = "$profile:\\";
	protected static LoggerManager m_LoggerManager;
	
	protected string m_FileName;
	
	void LoggerBase()
	{
		m_LoggerManager = LoggerManager.GetInstance();
	}
	
	// called in ctor of DayZGame
	void OnGameStart(notnull DayZGame game)
	{		
		if (!game) {
			return;
		}
		
		if ((GetLogMask() & LoggerInstanceMask.SERVER) != LoggerInstanceMask.SERVER && game.IsDedicatedServer()) {
			return;
		}
		
		if ((GetLogMask() & LoggerInstanceMask.CLIENT) != LoggerInstanceMask.CLIENT && !game.IsDedicatedServer()) {
			return;
		}
				
		// Create folder
		string folder_name = BASE_DIRECTORY + GetFolderName();
		if (!MakeDirectory(folder_name)) {
			Error("LoggerBase failed to make directory, exiting");
			return;
		}
		
		string file_name = GetFileFormat();
		file_name.Replace("%t%", Type().ToString());
		
		int y, m, d, hh, mm, ss;
		GetYearMonthDay(y, m, d);
		file_name.Replace("%y%", y.ToStringLen(2));
		file_name.Replace("%m%", m.ToStringLen(2));
		file_name.Replace("%d%", d.ToStringLen(2));
		
		GetHourMinuteSecond(hh, mm, ss);		
		file_name.Replace("%hh%", hh.ToStringLen(2));
		file_name.Replace("%mm%", mm.ToStringLen(2));
		file_name.Replace("%ss%", ss.ToStringLen(2));
		
		m_FileName = folder_name + "\\" + file_name + GetFileExtension();
				
		// Create the file
		CloseFile(OpenFile(m_FileName, FileMode.WRITE));
	}
			
	static void DoLog(typename logger_type, string log)
	{
		if (!GetGame()) {
			return;
		}
		
		LoggerBase logger_base = LoggerManager.GetInstance().GetLogger(logger_type);
		if (!logger_base) {
			Error("No logger found with type " + logger_type);
			return;
		}
				
		int log_mask = logger_base.GetLogMask();
		if ((log_mask & LoggerInstanceMask.SERVER) != LoggerInstanceMask.SERVER && GetGame().IsDedicatedServer()) {
			return;
		}
		
		if ((log_mask & LoggerInstanceMask.CLIENT) != LoggerInstanceMask.CLIENT && !GetGame().IsDedicatedServer()) {
			return;
		}

		FileHandle handle = OpenFile(logger_base.GetFileName(), FileMode.APPEND);
		FPrintln(handle, log);
		CloseFile(handle);
		
		if (logger_base.DuplicateToConsole()) {
			Print(String(log));
		}
		
		if (logger_base.DuplicateToRPT()) {
			PrintToRPT(String(log));
		}
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
	
	LoggerInstanceMask GetLogMask()
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
	
	// duplicates logs to the script console, not recommended for use on live servers
	bool DuplicateToConsole()
	{
		return false;
	}
	
	// duplicates logs to the RPT console, also not recommended for use on live servers
	bool DuplicateToRPT()
	{
		return false;
	}
}

/* 
	** Example Logger **
[RegisterLogger(KeycardLogger)]
class KeycardLogger: LoggerBase
{	
	override static void Log(string log)
	{
		DoLog(KeycardLogger, log);
	}
}
*/