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
	protected int m_StartTime = -1;
	protected string m_FunctionName;
	
	void ScopedFunctionTimer(string function_name)
	{
#ifdef DEBUG_SCOPED_FUNCTION_TIMERS
		m_FunctionName = function_name;
		m_StartTime = GetGame().GetTime();
#endif
	}	
	
	void ~ScopedFunctionTimer()
	{
#ifdef DEBUG_SCOPED_FUNCTION_TIMERS
		PrintFormat("%1: %2ms", m_FunctionName, GetGame().GetTime() - m_StartTime);
#endif
	}
	
	void Dump(string bookmark)
	{
#ifdef DEBUG_SCOPED_FUNCTION_TIMERS
		PrintFormat("\t%1 - %2: %3ms", m_FunctionName, bookmark, GetGame().GetTime() - m_StartTime);
#endif		
	}
}