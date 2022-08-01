modded class MissionBase
{
	// Not strong ref, handled by the EventManager itself
	// mainly used so people can do m_EventManager.RegisterEvent() in init.c
	protected EventManager m_EventManager;
	
	override void OnInit()
	{
		super.OnInit();
		
		m_EventManager = GetDayZGame().GetEventManager();
	}
	
	// Just another way to acquire it
	EventManager GetEventManager()
	{
		return m_EventManager;
	}
	
	// idk why this isnt a vanilla function, all the events are all over the place
	void OnPlayerJoined(PlayerBase player, PlayerIdentity identity)
	{
	}
}