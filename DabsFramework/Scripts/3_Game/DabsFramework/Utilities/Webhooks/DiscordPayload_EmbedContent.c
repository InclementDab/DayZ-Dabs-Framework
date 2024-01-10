class DiscordPayload_EmbedContent: Managed
{
	string url;
	int height;
	int width;
	
	void DiscordPayload_EmbedContent(string _url, int _height = 0, int _width = 0)
	{
		url = _url;
		height = _height;
		width = _width;
	}
}