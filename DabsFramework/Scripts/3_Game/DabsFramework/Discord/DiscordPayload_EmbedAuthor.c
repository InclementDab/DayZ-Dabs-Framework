class DiscordPayload_EmbedAuthor: Managed
{
	string name;
	string url;
	string icon_url;
	
	void DiscordPayload_EmbedAuthor(string _name, string _url = string.Empty, string _icon_url = string.Empty)
	{
		if (_name == string.Empty)
		{
			ErrorEx("Name can not be empty!");
		}
		
		name = _name;
		url = _url;
		icon_url = _icon_url;
	}
}