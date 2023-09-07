modded class DayZGame
{	
	protected static ref MVC s_MVC;
	protected ref LoggerManager m_LoggerManager;
	protected ref EventManager m_EventManager;
	
	protected ref map<typename, ProfileSettings> m_ProfileSettings = new map<typename, ProfileSettings>();
	
	void DayZGame()
	{		
		s_MVC = new MVC();
		m_LoggerManager = new LoggerManager(this);
		
		// dedi and offline
#ifdef SERVER
		m_EventManager = new EventManager();
#endif
	}
	
	void ~DayZGame()
	{		
		delete s_MVC;
		delete m_LoggerManager;
		delete m_EventManager;
		delete m_ProfileSettings;
	}
	
	override void OnUpdate(bool doSim, float timeslice)
	{
		super.OnUpdate(doSim, timeslice);

		if (doSim && m_EventManager) {
			m_EventManager.OnUpdate(timeslice);
		}
	}
		
	override void RegisterProfilesOptions()
	{
		super.RegisterProfilesOptions();
		
		// Load all ProfileSettings classes
		foreach (Param2<typename, string> profile_settings_type: RegisterProfileSetting.Instances) {
			ProfileSettings profile_settings = ProfileSettings.Cast(profile_settings_type.param1.Spawn());
			if (!profile_settings) {
				continue;
			}
			
			m_ProfileSettings[profile_settings_type.param1] = profile_settings;
		}
	}

#ifdef DIAG_DEVELOEPR	
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);
		
		switch (rpc_type) {
			case Debug.RPC_UPDATE_DEBUG_SHAPE: {
				Debug.OnDebugShapeRPC(ctx);		
				break;
			}
		}
	}
#endif
	
	// this is the init of event manager for clients
	override void OnEvent(EventType eventTypeId, Param params)
	{
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
		
		super.OnEvent(eventTypeId, params);
	}
	
	static MVC GetMVC()
	{
		if (!s_MVC) {
			s_MVC = new MVC();
		}
		
		return s_MVC;
	}
		
	DayZPlayer GetPlayerByIdentity(PlayerIdentity identity)
	{		
		int high, low;
		if (!IsMultiplayer()) {
			return GetPlayer();
		}
		
		if (!identity) {
			return null;
		}
		
		GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), low, high);
		return DayZPlayer.Cast(GetObjectByNetworkId(low, high));
	}
	
	string GetWorldNameEx(bool format = true)
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		if (format) {
			world_name.ToLower();
		}
		
		return world_name;
	}
	
	LoggerManager GetLoggerManager()
	{
		return m_LoggerManager;
	}
	
	EventManager GetEventManager()
	{
		return m_EventManager;
	}
	
	ProfileSettings GetProfileSetting(typename profile_settings_type)
	{
		return m_ProfileSettings[profile_settings_type];
	}
}