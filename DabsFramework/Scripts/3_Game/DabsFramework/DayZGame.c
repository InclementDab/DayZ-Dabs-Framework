modded class DayZGame
{	
	protected static ref MVC s_MVC;
	protected ref LoggerManager m_LoggerManager;
	protected ref EventManager m_EventManager;
	
	void DayZGame()
	{		
		s_MVC = new MVC();
		m_LoggerManager = new LoggerManager(this);
		
		// dedi and offline
		if (IsServer()) {
			m_EventManager = new EventManager();
		}
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
	
	EventManager GetEventManager()
	{
		return m_EventManager;
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
	
	override void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);

		switch (eventTypeId) {
			case MPSessionStartEventTypeID: {
				m_EventManager = new EventManager();
				break;
			}
			
			case MPSessionEndEventTypeID: {
				delete m_EventManager;
				break;
			}
		}
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