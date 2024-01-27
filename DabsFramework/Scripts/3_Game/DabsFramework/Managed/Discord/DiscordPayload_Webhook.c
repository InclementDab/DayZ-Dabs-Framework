class DiscordPayload_Webhook: Managed
{
	string username;
	string content;
	string avatar_url;
	ref array<ref DiscordPayload_Embed> embeds = {};
	string thread_name;
	
	void DiscordPayload_Webhook(string _username = string.Empty, string _content = string.Empty, string _avatar_url = string.Empty, array<ref DiscordPayload_Embed> _embeds = null, string _thread_name = string.Empty)
	{
		username = _username;
		content = _content;
		avatar_url = _avatar_url;
		if (_embeds) {
			embeds = _embeds;
		}
		
		thread_name = _thread_name;
	}

	void SetUser(string _username, string _avatar_url = string.Empty)
	{
		username = _username;
		avatar_url = _avatar_url;
	}

	void SetContent(string _content)
	{
		content = _content;
	}

	void SetThreadName(string _thread_name)
	{
		thread_name = _thread_name;
	}

	DiscordPayload_Embed CreateEmbed(string _title, string _description)
	{
		DiscordPayload_Embed embed = new DiscordPayload_Embed(_title, _description);
		AddEmbed(embed);

		return embed;
	}

	void AddEmbed(DiscordPayload_Embed embed)
	{
		if (!embeds) {
			embeds = new array<ref DiscordPayload_Embed>();
		}

		if (embeds.Count() > 9) {
			ErrorEx("Maximum number of embeds allowed is 10!");
			return;
		}

		embeds.Insert(embed);
	}
}