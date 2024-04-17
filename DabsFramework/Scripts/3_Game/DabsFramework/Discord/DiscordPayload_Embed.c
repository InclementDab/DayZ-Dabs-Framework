class DiscordPayload_Embed: Managed
{
	string title;
	string description;
	string url;
	string timestamp;
	int color;
		
	ref DiscordPayload_EmbedFooter footer;
	ref DiscordPayload_EmbedContent image;
	ref DiscordPayload_EmbedContent thumbnail;
	ref DiscordPayload_EmbedContent video;
	ref DiscordPayload_EmbedProvider provider;
	ref DiscordPayload_EmbedAuthor author;
	
	ref array<ref DiscordPayload_EmbedField> fields = {};
	
	void DiscordPayload_Embed(string _title, string _description, string _url = string.Empty, int _color = 0, array<ref DiscordPayload_EmbedField> _fields = null)
	{
		title = _title;
		description = _description;
		url = _url;
		color = _color;
		if (_fields) {
			fields = fields;
		}
	}

	void SetTitle(string _title)
	{
		if (_title == string.Empty)
		{
			ErrorEx("Title can not be empty!");
		}
		
		title = _title;
	}

	void SetDescription(string _description)
	{
		if (_description == string.Empty)
		{
			ErrorEx("Description can not be empty!");
		}

		description = _description;
	}

	void SetUrl(string _url)
	{
		url = _url;
	}

	void SetColor(int r, int g, int b)
	{
		color = ARGB(0, r, g, b);
	}

	void SetColor(int _color)
	{
		color = _color;
	}

	void SetAuthor(string _name, string _url = string.Empty, string _icon_url = string.Empty)
	{
		author = new DiscordPayload_EmbedAuthor(_name, _url, _icon_url);
	}

	DiscordPayload_EmbedField CreateField(string _name, string _value, bool _inline = false)
	{
		DiscordPayload_EmbedField field = new DiscordPayload_EmbedField(_name, _value, _inline);
		AddField(field);

		return field;
	}

	void AddField(DiscordPayload_EmbedField _field)
	{
		if (!fields) {
			fields = new array<ref DiscordPayload_EmbedField>();
		}

		fields.Insert(_field);
	}

	void SetThumbnail(string _url, int _height = 0, int _width = 0)
	{
		thumbnail = new DiscordPayload_EmbedContent(_url, _height, _width);
	}

	void SetImage(string _url, int _height = 0, int _width = 0)
	{
		image = new DiscordPayload_EmbedContent(_url, _height, _width);
	}

	void SetVideo(string _url, int _height = 0, int _width = 0)
	{
		image = new DiscordPayload_EmbedContent(_url, _height, _width);
	}

	void SetProvider(string _name, string _url = string.Empty)
	{
		provider = new DiscordPayload_EmbedProvider(_name, _url);
	}

	void SetFooter(string footerText, string footerIcon = string.Empty)
	{
		footer = new DiscordPayload_EmbedFooter(footerText, footerIcon);
	}

	void SetTimestamp(DateTime dateTime)
	{
		timestamp = dateTime.ToString(DateTime.FORMAT_ISO_DATETIME);
	}
}