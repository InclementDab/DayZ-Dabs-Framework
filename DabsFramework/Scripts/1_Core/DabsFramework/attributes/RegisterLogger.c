//@ RegisterLogger attribute, see LoggerBase for usage example
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