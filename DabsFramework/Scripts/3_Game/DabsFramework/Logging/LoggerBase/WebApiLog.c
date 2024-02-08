[RegisterLogger(WebApiLog)]
class WebApiLog: LoggerBase
{
	static override void Log(string log)
	{
		DoLog(WebApiLog, log);
	}
	
	static void Trace(Class source, string function)
	{
#ifdef DIAG
		string stack_trace;
		DumpStackString(stack_trace);
		array<string> stack_trace_lines = {};
		stack_trace.Split("\n", stack_trace_lines);

		DoLog(WebApiLog, string.Format("[%1::%2] %3", source, function, stack_trace_lines[0]));
#endif
	}

	static void Error(string error)
	{
		Print("RearmedAPI threw an error");
		DumpStack();
		
		DoLog(WebApiLog, string.Format("[ERROR] %1", error));

		string stack_trace;
		DumpStackString(stack_trace);
		DoLog(WebApiLog, stack_trace); 
	}

#ifdef DIAG
	override bool DuplicateToConsole()
	{
		return true;
	}
#endif
	
	override bool DuplicateToRPT()
	{
		return true;
	}	
	
	override LoggerInstanceMask GetLogMask()
	{
		return LoggerInstanceMask.SERVER | LoggerInstanceMask.CLIENT;
	}
}