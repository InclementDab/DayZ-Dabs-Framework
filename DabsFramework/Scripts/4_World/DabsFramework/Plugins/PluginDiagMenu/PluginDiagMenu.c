#ifdef DIAG_DEVELOPER

modded class PluginDiagMenu
{	
	static const int RPC_DIAG_RESET_PROFILER = 3566037;
	static const int RPC_DIAG_DUMP_PROFILER = 3566038;
	
	protected static int m_DabsFrameworkRootID;
		protected static int m_ResetProfilerID;
		protected static int m_DumpProfilerID;

	protected override void RegisterModdedDiagsIDs()
	{
		super.RegisterModdedDiagsIDs();
		
		m_DabsFrameworkRootID = GetModdedDiagID();
			m_ResetProfilerID = GetModdedDiagID();
			m_DumpProfilerID = GetModdedDiagID();
	}
		
	protected override void RegisterModdedDiags()
	{
		super.RegisterModdedDiags();
		
		DiagMenu.RegisterMenu(m_DabsFrameworkRootID, "Dabs Framework", DiagMenuIDs.MODDED_MENU);
		{
			DiagMenu.RegisterItem(m_ResetProfilerID, "[", "Reset Profiler", m_DabsFrameworkRootID, "");
			DiagMenu.RegisterItem(m_DumpProfilerID, "]", "Dump Profiler", m_DabsFrameworkRootID, "");
		}
	}
		
	override void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(player, rpc_type, ctx);
		
		switch (rpc_type) {						
			case RPC_DIAG_RESET_PROFILER: {
				EnProfiler.ResetSession(true);
				break;
			}
			
			case RPC_DIAG_DUMP_PROFILER: {
				EnProfiler.Dump();
				break;
			}
		}
	}
}

#endif

