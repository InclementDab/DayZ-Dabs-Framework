class DiscordPayload_EmbedFooter: Managed
{
	string text;
	string icon_url;
	
	void DiscordPayload_EmbedFooter(string _text, string _icon_url = string.Empty)
	{
		if (_text == string.Empty)
		{
			ErrorEx("Text can not be empty!");
		}
		
		text = _text;
		icon_url = _icon_url;
	}
}