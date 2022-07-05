modded class DayZGame
{	
	protected ref MVC m_MVC;
	protected ref LoggerManager m_LoggerManager;
	
	void DayZGame()
	{		
		m_MVC = new MVC();
		m_LoggerManager = new LoggerManager(this);
	}
	
	void ~DayZGame()
	{
		delete m_MVC;
		delete m_LoggerManager;
	}
	
	MVC GetMVC()
	{
		return m_MVC;
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