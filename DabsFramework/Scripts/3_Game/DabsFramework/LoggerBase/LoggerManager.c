class LoggerManager
{
	protected ref map<typename, ref LoggerBase> m_Loggers = new map<typename, ref LoggerBase>();
	
	void LoggerManager(notnull DayZGame game)
	{
		foreach (typename logger_instance_type: RegisterLogger.RegisteredLoggers) {
			RegisterLogger(logger_instance_type);
		}
		
		foreach (typename logger_type, LoggerBase logger_instance: m_Loggers) {
			if (logger_instance) {
				logger_instance.OnGameStart(game);
			}
		}
	}
	
	void ~LoggerManager()
	{
		delete m_Loggers;
	}
	
	void RegisterLogger(typename logger_type)
	{
		m_Loggers[logger_type] = LoggerBase.Cast(logger_type.Spawn());
	}
	
	LoggerBase GetLogger(typename logger)
	{
		return m_Loggers[logger];
	}
	
	static LoggerManager GetInstance()
	{
		return GetDayZGame().GetLoggerManager();
	}
}