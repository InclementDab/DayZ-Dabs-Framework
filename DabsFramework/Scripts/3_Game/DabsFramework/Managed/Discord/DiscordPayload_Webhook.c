class DiscordPayload_Webhook: Managed
{
	string username;
	string content;
	string avatar_url;
	ref array<ref DiscordPayload_Embed> embeds = {}; // max of 10
	string thread_name;
	
	void DiscordPayload_Webhook(string _username, string _content, string _avatar_url, array<ref DiscordPayload_Embed> _embeds = null, string _thread_name = string.Empty)
	{
		username = _username;
		content = _content;
		avatar_url = _avatar_url;
		if (_embeds) {
			embeds = _embeds;
		}
		
		thread_name = _thread_name;
	}
}