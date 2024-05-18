#ifdef DIAG_DEVELOPER

modded class PluginDiagMenu
{	
	static const int RPC_DIAG_RESET_PROFILER = 3566037;
	static const int RPC_DIAG_DUMP_PROFILER = 3566038;
	
	override void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(player, rpc_type, ctx);
		
#ifdef DIAG
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
#endif
	}
}

#endif

