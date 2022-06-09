#define EVENT_MANAGER_DEBUG

[RegisterLogger(EventManagerLog)]
class EventManagerLog: LoggerBase
{
	// inst is of type EventBase
	static void EventDebug(Class inst, string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EVENT_MANAGER_DEBUG		
		Log("[DF][" + inst.Type() + "][DEBUG]: " + string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9));
#endif
	}
	
	static void EventInfo(Class inst, string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		Log("[DF][" + inst.Type() + "][INFO]: " + string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
	
	static void Info(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		Log("[DF][INFO]: " + string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
	
	static void Debug(string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EVENT_MANAGER_DEBUG
		Log("[DF][DEBUG]: " + string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9));
#endif
	}
	
	override void Log(string log)
	{
		DoLog(EventManagerLog, log);
	}
	
#ifdef EVENT_MANAGER_DEBUG
	override bool DuplicateToConsole()
	{
		return true;
	}
#endif
}