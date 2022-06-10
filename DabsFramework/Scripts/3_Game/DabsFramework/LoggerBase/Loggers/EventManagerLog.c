[RegisterLogger(EventManagerLog)]
class EventManagerLog: LoggerBase
{
	// inst is of type EventBase
	static void Debug(Class inst, string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
#ifdef EVENT_MANAGER_DEBUG
		string inst_type;
		if (inst) {
			inst_type = "[" + inst.Type() + "]";
		}
		
		Log("[DF][DEBUG]" + inst_type + ": " + string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9));
#endif
	}
	
	static void Info(Class inst, string msg, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "")
	{
		string inst_type;
		if (inst) {
			inst_type = "[" + inst.Type() + "]";
		}
		
		Log("[DF][INFO]" + inst_type + ": " + string.Format(msg, param1, param2, param3, param4, param5, param6, param7, param8, param9));
	}
		
	static override void Log(string log)
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