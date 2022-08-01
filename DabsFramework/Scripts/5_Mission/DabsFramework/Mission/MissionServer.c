modded class MissionServer
{
	protected ref array<string> m_JoinedPlayers = {};
	
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
	}
	
	override void OnClientDisconnectedEvent(PlayerIdentity identity, PlayerBase player, int logoutTime, bool authFailed)
	{
		super.OnClientDisconnectedEvent(identity, player, logoutTime, authFailed);
		
		if (m_JoinedPlayers.Find(identity.GetId()) != -1) {
			m_JoinedPlayers.Remove(m_JoinedPlayers.Find(identity.GetId()));
		}
	}
	
	// idk why this isnt a vanilla function, all the events are all over the place
	void OnPlayerJoined(PlayerBase player, PlayerIdentity identity)
	{
	}
}