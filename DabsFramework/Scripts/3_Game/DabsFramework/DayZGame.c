modded class DayZGame
{	
	protected static ref MVC s_MVC;
	protected ref LoggerManager m_LoggerManager;
	
	void DayZGame()
	{		
		s_MVC = new MVC();
		m_LoggerManager = new LoggerManager(this);
	}
	
	void ~DayZGame()
	{
		delete s_MVC;
		delete m_LoggerManager;
	}
	
	static MVC GetMVC()
	{
		if (!s_MVC) {
			s_MVC = new MVC();
		}
		
		return s_MVC;
	}
	
	LoggerManager GetLoggerManager()
	{
		return m_LoggerManager;
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);
		
#ifdef DIAG
		switch (rpc_type) {
			case Debug.RPC_UPDATE_DEBUG_SHAPE: {
				Debug.OnDebugShapeRPC(ctx);		
				break;
			}
		}
#endif
	}
	
	DayZPlayer GetPlayerByIdentity(notnull PlayerIdentity identity)
	{
		int high, low;
		if (!IsMultiplayer()) {
			return GetPlayer();
		}
		
		GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), low, high);
		return DayZPlayer.Cast(GetObjectByNetworkId(low, high));
	}
}