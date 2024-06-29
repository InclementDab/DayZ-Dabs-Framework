class Directory: FileSystem
{		
	static array<string> EnumerateFiles(string directory, string filter = "*")
	{
		array<string> enumerated_files = {};
		string file_name;
		FileAttr file_attributes;
		FindFileHandle handle = FindFile(directory + SystemPath.SEPERATOR + "*", file_name, file_attributes, FindFileFlags.ALL);
		if (!handle) {
			return enumerated_files;
		}
		
		if (File.WildcardMatch(file_name, filter)) {
			enumerated_files.Insert(directory + SystemPath.SEPERATOR + file_name);
		}
		
		while (FindNextFile(handle, file_name, file_attributes)) {
			if (File.WildcardMatch(file_name, filter)) {
				enumerated_files.Insert(directory + SystemPath.SEPERATOR + file_name);
			}
		}
		
		CloseFindFile(handle);
		return enumerated_files;
	}
	
	// Gets directory of file
	static string GetDirectory(string file_input)
	{
		file_input.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		array<string> path_seperated = {};
		file_input.Split(SystemPath.SEPERATOR, path_seperated);
		if (path_seperated.Count() <= 1) {
			return file_input;
		}
		
		path_seperated.RemoveOrdered(path_seperated.Count() - 1);
		return string.Join(SystemPath.SEPERATOR, path_seperated);
	}
}

typedef string Directory;