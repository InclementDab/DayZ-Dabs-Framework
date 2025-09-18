#ifdef DIAG_DEVELOPER
#define DEBUG_SCOPED_FUNCTION_TIMERS
#endif

/*
	ScopedFunctionTimer Example

	class MyProfiledClass
	{
		void Update()
		{
			ScopedFunctionTimer Scope("MyProfileClass.Update");
			
			... do some stuff
		
			Scope.Dump("Checkpoint 1");
		
			... do some more stuff
		}	
	}

	* the output will be

	MyProfileClass.Update - Checkpoint 1: 12ms
	MyProfileClass.Update 25ms

	* maybe one day this can be a binary file output and ill stick it into unreal CF
		if you want to enable the profiling in the main game build, just define DEBUG_SCOPED_FUNCTION_TIMERS in your scrdefines

*/

class ScopedFunctionTimer: Managed
{
	private static ref map<string, int> s_TotalFunctionTime = new map<string, int>();
	
	protected int m_StartTime = -1, m_LastDumpTime = -1;
	protected int m_StartTick;
	protected string m_FunctionName;
	protected int m_ActionIncrement;
	
	void ScopedFunctionTimer(string function_name = string.Empty)
	{
#ifdef DEBUG_SCOPED_FUNCTION_TIMERS
		if (function_name == string.Empty) {
			string stack;
			DumpStackString(stack);
			
			Print(stack);
		}
		
		m_FunctionName = function_name;
		m_StartTime = g_Game.GetTime();
		m_StartTick = TickCount(0);
		m_LastDumpTime = m_StartTime;
#endif
	}	
	
	void ~ScopedFunctionTimer()
	{
#ifdef DEBUG_SCOPED_FUNCTION_TIMERS
		string average_time_per_action;
		int tick_total = TickCount(m_StartTick);
		if (m_ActionIncrement) {
			average_time_per_action = string.Format(" (Average Time Per Action: [%1ms, %2 ticks each])", (g_Game.GetTime() - m_LastDumpTime) / m_ActionIncrement, tick_total / m_ActionIncrement);
		}
		
		int delta_time = g_Game.GetTime() - m_StartTime;
		PrintFormat("%1: %2ms (%4 total ticks)%3", m_FunctionName, delta_time, average_time_per_action, tick_total);
		
		if (!s_TotalFunctionTime) {
			s_TotalFunctionTime = new map<string, int>();
		}
		
		s_TotalFunctionTime[m_FunctionName] = s_TotalFunctionTime[m_FunctionName] + delta_time;
#endif
	}
	
	void Dump(string bookmark)
	{
#ifdef DEBUG_SCOPED_FUNCTION_TIMERS
		string average_time_per_action;
		if (m_ActionIncrement) {
			float denominator = 1;
			if (g_Game.GetTime() != m_LastDumpTime) {
				denominator = g_Game.GetTime() - m_LastDumpTime;
			}
			average_time_per_action = string.Format(" (Average Time Per Action: [%1ms])", m_ActionIncrement / denominator);
		}

		m_ActionIncrement = 0;

		PrintFormat("\t%1 - %2: %3ms inc, %4ms total, %5", m_FunctionName, bookmark, g_Game.GetTime() - m_LastDumpTime, g_Game.GetTime() - m_StartTime, average_time_per_action);
		m_LastDumpTime = g_Game.GetTime();
#endif		
	}

	void IncrementAction()
	{
		m_ActionIncrement++;
	}
	
	// Gets the total time from a single function
	static int GetTotalTime(string function_name)
	{
		return s_TotalFunctionTime[function_name];
	}
	
	// Dumps total inclusive time of a function
	static void DumpTotalTime(string function_name)
	{
		PrintFormat("[Total] %1: %2ms", function_name, s_TotalFunctionTime[function_name]);
	}
}