class DiscordPayload_Embed: Managed
{
	//string timestamp = DateTime.Now().ToString(DateTime.FORMAT_ISO_DATETIME);
	
	string title;
	string description;
	string url;
	int color;
		
	ref DiscordPayload_EmbedFooter footer;
	ref DiscordPayload_EmbedContent image;
	ref DiscordPayload_EmbedContent thumbnail;
	ref DiscordPayload_EmbedContent video;
	ref DiscordPayload_EmbedProvider provider;
	ref DiscordPayload_EmbedAuthor author;
	
	ref array<ref DiscordPayload_EmbedField> fields = {};
	
	void DiscordPayload_Embed(string _title, string _description, string _url, int _color, array<ref DiscordPayload_EmbedField> _fields = null)
	{
		title = _title;
		description = _description;
		url = _url;
		color = _color;
		if (_fields) {
			fields = fields;
		}
	}
}