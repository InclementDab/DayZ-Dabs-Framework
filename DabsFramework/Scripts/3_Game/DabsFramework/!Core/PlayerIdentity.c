modded class PlayerIdentity
{
	// use PlayerIdentity::GetId()
	static PlayerIdentity FindById(string id)
	{
		array<PlayerIdentity> player_identities = {};
		GetDayZGame().GetPlayerIndentities(player_identities);
		foreach (PlayerIdentity player_identity: player_identities) {
			if (player_identity.GetId() == id) {
				return player_identity;
			}
		}
		
		return null;
	}
}