class DiscordPayload_EmbedFooter: Managed
{
	string text;
	string icon_url;
	
	void DiscordPayload_EmbedFooter(string _text, string _icon_url = string.Empty)
	{
		text = _text;
		icon_url = _icon_url;
	}
}