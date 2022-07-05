// Attribute
class RegisterLogger
{
	static ref array<typename> RegisteredLoggers = {};
	
	void RegisterLogger(typename logger)
	{
		if (!RegisteredLoggers) {
			RegisteredLoggers = {};
		}
				
		RegisteredLoggers.Insert(logger);
	}
}