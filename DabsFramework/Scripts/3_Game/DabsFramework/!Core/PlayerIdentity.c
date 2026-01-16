modded class PlayerIdentity
{
	// use PlayerIdentity::GetId()
	static PlayerIdentity FindById(string id)
	{
		array<PlayerIdentity> player_identities = {};
		GetDayZGame().GetPlayerIndentities(player_identities);
		for (int i = 0; i < player_identities.Count(); i++)
        {
            if (player_identities[i].GetId() == id)
            {
                return player_identities[i];
            }
        }
		
		return null;
	}
	
	// use PlayerIdentity::GetPlainId()
	static PlayerIdentity FindByPlainId(string plain_id)
	{
		array<PlayerIdentity> player_identities = {};
		GetDayZGame().GetPlayerIndentities(player_identities);
		for (int i = 0; i < player_identities.Count(); i++)
        {
            if (player_identities[i].GetPlainId() == plain_id)
            {
                return player_identities[i];
            }
        }
		
		return null;
	}

	static PlayerIdentity FindByName(string name)
	{
		array<PlayerIdentity> player_identities = {};
		GetDayZGame().GetPlayerIndentities(player_identities);
		for (int i = 0; i < player_identities.Count(); i++)
        {
            if (player_identities[i].GetName() == name)
            {
                return player_identities[i];
            }
        }

		return null;
	}
}