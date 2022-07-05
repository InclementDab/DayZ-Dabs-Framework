modded class DayZGame
{	
	protected ref MVC m_MVC;
	protected LoggerManager m_LoggerManager;
	
	void DayZGame()
	{
		m_MVC = new MVC();
		
		// get instance of logger manager
		m_LoggerManager = LoggerManager.GetInstance();
		
		// this is likely if no loggers are registered
		if (!m_LoggerManager) {
			m_LoggerManager = LoggerManager.Start();
		}
		
		m_LoggerManager.OnGameStart(this);
	}
	
	void ~DayZGame()
	{
		delete m_MVC;
		LoggerManager.Stop();
	}
	
	MVC GetMVC()
	{
		return m_MVC;
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