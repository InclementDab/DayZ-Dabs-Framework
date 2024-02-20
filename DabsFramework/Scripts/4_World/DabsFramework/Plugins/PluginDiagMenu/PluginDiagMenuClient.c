#ifdef DIAG_DEVELOPER

modded class PluginDiagMenuClient
{	
	protected override void BindCallbacks()
	{
		super.BindCallbacks();

		DiagMenu.BindCallback(m_ResetProfilerID, CBResetProfiler);
		DiagMenu.BindCallback(m_DumpProfilerID, CBDumpProfiler);
	}
	
	static void CBResetProfiler(int level)
	{
		SendDiagRPC(level, RPC_DIAG_RESET_PROFILER, true);
	}
		
	static void CBDumpProfiler(int level)
	{
		SendDiagRPC(level, RPC_DIAG_DUMP_PROFILER, true);
	}
		
	static void SendDiagRPC(string value, int rpc, bool serverOnly = false)
	{
		CachedObjectsParams.PARAM1_STRING.param1 = value;
		SendDiagRPC(CachedObjectsParams.PARAM1_STRING, rpc, serverOnly);
 	}
}

#endif