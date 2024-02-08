//@ RegisterLogger attribute, see LoggerBase for usage example
class RegisterLogger: Class
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