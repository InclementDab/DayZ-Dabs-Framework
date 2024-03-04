class FileSystem: string
{
	static const string WILDCARD = "*";
	
	static const string PATH_SEPERATOR = "\\";
	static const string PATH_SEPERATOR_ALT = "/";
	static const string VOLUME_SEPERATOR = ":";
	
	static const string PROFILES = "$profiles:";
	static const string MISSION = "$mission:";
	static const string SAVES = "$saves:";
	static const string STORAGE = "$storage:";
	
	string GetFullPath()
	{
		return value;
	}
	
	string GetExtension()
	{
		for (int i = value.Length(); --i >= 0; ) {
			switch (value[i]) {
				case ".": {
					return value.Substring(i, value.Length() - i);
					break;
				}
				
				case PATH_SEPERATOR:
				case PATH_SEPERATOR_ALT:
				case VOLUME_SEPERATOR: {
					return string.Empty;
				}
			}
		}
		
		return string.Empty;
	}
	
	bool Exists()
	{
		return FileExist(value);
	}
}

typedef string FileSystem;