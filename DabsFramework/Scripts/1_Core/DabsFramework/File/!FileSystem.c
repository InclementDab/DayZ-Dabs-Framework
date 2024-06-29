class FileSystem: string
{
	const string WILDCARD = "*";
	
	string GetFileName()
	{
		string temp = value;
		temp.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		
		array<string> path_split = {};
		temp.Split(SystemPath.SEPERATOR, path_split);
		
		if (path_split.Count() == 0) {
			return string.Empty;
		}
		
		return path_split[path_split.Count() - 1];
	}
	
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
				
				case SystemPath.SEPERATOR:
				case SystemPath.SEPERATOR_ALT:
				case SystemPath.VOLUME_PREFIX:
				case SystemPath.VOLUME_SEPERATOR: {
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