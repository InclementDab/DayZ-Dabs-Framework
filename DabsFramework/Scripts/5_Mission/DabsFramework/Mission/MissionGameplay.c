modded class MissionGameplay
{
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		// Clear client events once the server has been left.
		EventManager event_manager = GetDayZGame().GetEventManager();
		if (event_manager) {
			event_manager.CleanupEvents();
		}
	}
}