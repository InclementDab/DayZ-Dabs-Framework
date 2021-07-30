modded class DayZGame
{
	DayZPlayer GetPlayerByIdentity(notnull PlayerIdentity identity)
	{
		int high, low;
		if (IsMultiplayer()) {
			return GetPlayer();
		}
		
		GetPlayerNetworkIDByIdentityID(identity.GetPlayerId(), low, high);
		return DayZPlayer.Cast(GetObjectByNetworkId(low, high));
	}
}