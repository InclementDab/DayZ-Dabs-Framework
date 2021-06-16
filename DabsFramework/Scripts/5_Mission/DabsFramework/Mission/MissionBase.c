modded class MissionBase
{
	// Not strong ref, handled by the EventManager itself
	protected EventManager m_EventManager;
	
	override void OnInit()
	{
		super.OnInit();
		
		m_EventManager = EventManager.Start();
	}
}