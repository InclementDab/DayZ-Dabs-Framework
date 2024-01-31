modded class MissionServer
{
	protected ref array<string> m_JoinedPlayers = {};
	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(AfterHiveInit, 0, false, GetCEApi());
	}
	
	// Called immediately after main() is done, CLE is initialized by this point
	void AfterHiveInit(Hive hive)
	{
	}
	
	// load client info first hand
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		
		if (m_JoinedPlayers.Find(identity.GetId()) == -1) {
			OnPlayerJoined(player, identity);
			m_JoinedPlayers.Insert(identity.GetId());
		}
				
		// JIP Event Data, post review, this may only need to be done in OnPlayerJoined
		if (m_EventManager) {
			m_EventManager.DispatchEventInfo(player);
		}
		
		SuppressedObjectManager supressed_object_manager = GetDayZGame().GetSuppressedObjectManager();
		if (supressed_object_manager) {
			supressed_object_manager.DispatchInfo(identity);
		}
	}
	
	override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
	{
		super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
		
		if (m_JoinedPlayers.Find(identity.GetId()) != -1) {
			m_JoinedPlayers.Remove(m_JoinedPlayers.Find(identity.GetId()));
		}
	}
}