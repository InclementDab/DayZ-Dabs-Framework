class UUID: string
{
	static const UUID EMPTY = "00000000-0000-0000-0000-00000000";	
	static UUID Generate()
	{
		int uid[4];
		UUIDApi.Generate(uid);
		return UUIDApi.FormatString(uid);
	}
}

typedef string UUID;