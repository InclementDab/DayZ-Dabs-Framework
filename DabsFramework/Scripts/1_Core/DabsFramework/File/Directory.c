class Directory: FileSystem
{		
	static array<string> EnumerateFiles(string directory, string filter = "*")
	{
		array<string> enumerated_files = {};
		string file_name;
		FileAttr file_attributes;
		FindFileHandle handle = FindFile(directory + PATH_SEPERATOR + "*", file_name, file_attributes, FindFileFlags.ALL);
		if (!handle) {
			return enumerated_files;
		}
		
		if (File.WildcardMatch(file_name, filter)) {
			enumerated_files.Insert(directory + PATH_SEPERATOR + file_name);
		}
		
		while (FindNextFile(handle, file_name, file_attributes)) {
			if (File.WildcardMatch(file_name, filter)) {
				enumerated_files.Insert(directory + PATH_SEPERATOR + file_name);
			}
		}
		
		CloseFindFile(handle);
		return enumerated_files;
	}
}

typedef string Directory;