#ifdef DIAG_DEVELOPER

modded class PluginDiagMenuClient
{	
	protected override void BindCallbacks()
	{
		super.BindCallbacks();

		DiagMenu.BindCallback(m_TeleportToCursorID, CBTeleportToCursor);
		DiagMenu.BindCallback(m_ResetProfilerID, CBResetProfiler);
		DiagMenu.BindCallback(m_DumpProfilerID, CBDumpProfiler);
	}
		
	static void CBTeleportToCursor(int value)
	{		
		vector pos, dir;
		int comp;
		if (DayZPhysics.RaycastRV(GetGame().GetCurrentCameraPosition(), GetGame().GetCurrentCameraPosition() + GetGame().GetCurrentCameraDirection() * 1000, pos, dir, comp, null, null, GetGame().GetPlayer())) {
			SendDiagRPC(pos.ToString(false), RPC_DIAG_TELEPORT_CURSOR, true);
		}
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