class DiscordPayload_EmbedProvider: Managed
{
	string name;
	string url;
	
	void DiscordPayload_EmbedProvider(string _name, string _url = string.Empty)
	{
		if (_name == string.Empty)
		{
			ErrorEx("Name can not be empty!");
		}
		
		name = _name;
		url = _url;
	}
}