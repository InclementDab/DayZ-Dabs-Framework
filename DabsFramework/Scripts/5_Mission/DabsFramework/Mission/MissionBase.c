modded class MissionBase
{
	protected ref EventManager m_EventManager;
	
	override void OnInit()
	{
		super.OnInit();
		
		m_EventManager = new EventManager();
	}

	EventManager GetEventManager()
	{
		return m_EventManager;
	}
}