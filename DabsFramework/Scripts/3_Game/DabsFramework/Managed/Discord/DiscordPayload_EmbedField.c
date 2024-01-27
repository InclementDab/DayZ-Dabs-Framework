class DiscordPayload_EmbedField: Managed
{
	string name;
	string value;
	bool inline;
	
	void DiscordPayload_EmbedField(string _name, string _value, bool _inline = false)
	{
		if (_name == string.Empty)
		{
			ErrorEx("Name can not be empty!");
		}
		
		if (_value == string.Empty)
		{
			ErrorEx("Value can not be empty!");
		}
		
		name = _name;
		value = _value;
		inline = _inline;
	}
}