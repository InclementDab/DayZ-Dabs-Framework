modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		
		// JIP Event Data
		m_EventManager.DispatchEventInfo(player);
	}
}