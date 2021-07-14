modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		
		// JIP Event Data
		if (m_EventManager) {
			m_EventManager.DispatchEventInfo(player);
		}
	}
}