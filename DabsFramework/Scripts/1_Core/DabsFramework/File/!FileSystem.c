class FileSystem: string
{
	const string WILDCARD = "*";
	
	string GetFileName()
	{
		string temp = value;
		temp.Replace(Path.SEPERATOR_ALT, Path.SEPERATOR);
		
		array<string> path_split = {};
		temp.Split(Path.SEPERATOR, path_split);
		
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
				
				case Path.SEPERATOR:
				case Path.SEPERATOR_ALT:
				case Path.VOLUME_PREFIX:
				case Path.VOLUME_SEPERATOR: {
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