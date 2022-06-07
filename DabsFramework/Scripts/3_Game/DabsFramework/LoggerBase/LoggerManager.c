// Attribute
class RegisterLogger
{
	void RegisterLogger(typename logger)
	{
		LoggerManager logger_manager = LoggerManager.GetInstance();
		if (!logger_manager) {
			logger_manager = LoggerManager.Start();
		}
		
		logger_manager.RegisterLogger(logger);
	}
}

class LoggerManager
{
	protected ref map<typename, ref LoggerBase> m_Loggers = new map<typename, ref LoggerBase>();
		
	void RegisterLogger(typename logger_type)
	{
		m_Loggers[logger_type] = LoggerBase.Cast(logger_type.Spawn());
	}
	
	LoggerBase GetLogger(typename logger)
	{
		return m_Loggers[logger];
	}
	
	void OnGameStart(DayZGame game)
	{
		foreach (typename logger_type, LoggerBase logger_instance: m_Loggers) {
			if (logger_instance) {
				logger_instance.OnGameStart(game);
			}
		}
	}
	
	// Static singleton stuff
	protected static ref LoggerManager m_Instance;
	
	static LoggerManager Start()
	{
		if (!m_Instance) {
			m_Instance = new LoggerManager();
		}
		
		return m_Instance;
	}
	
	static void Stop()
	{
		delete m_Instance;
	}
	
	static LoggerManager GetInstance()
	{
		return m_Instance;
	}
}