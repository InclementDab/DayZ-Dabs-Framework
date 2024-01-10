class DiscordPayload_EmbedField: Managed
{
	string name;
	string value;
	bool inline;
	
	void DiscordPayload_EmbedField(string _name, string _value, bool _inline = false)
	{
		name = _name;
		value = _value;
		inline = _inline;
	}
}