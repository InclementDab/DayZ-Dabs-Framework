class Directory: FileSystem
{		
	static array<string> EnumerateDirectories(string directory)
	{
		array<string> enumerated_directories = {};
		string folder_name;
		FileAttr folder_attributes;
		FindFileHandle handle = FindFile(directory + SystemPath.SEPERATOR, folder_name, folder_attributes, FindFileFlags.ALL | FindFileFlags.DIRECTORIES | FindFileFlags.ARCHIVES);
		if (!handle) {
			return enumerated_directories;
		}
		
		if (folder_attributes & FileAttr.DIRECTORY) {
			enumerated_directories.Insert(SystemPath.Combine(directory, folder_name));
		}

		while (FindNextFile(handle, folder_name, folder_attributes)) {
			if (folder_attributes & FileAttr.DIRECTORY) {
				enumerated_directories.Insert(SystemPath.Combine(directory, folder_name));
			}
		}
		
		CloseFindFile(handle);
		return enumerated_directories;
	}
	
	static array<string> EnumerateFiles(string directory, string filter = "*", int recursion_depth = 0)
	{
		array<string> enumerated_files = {}; 
		string file_name;
		FileAttr file_attributes;
		if (recursion_depth > 0) {
			array<string> enumerated_directories = Directory.EnumerateDirectories(directory);
			enumerated_directories.Debug();
			foreach (string enumerated_directory: enumerated_directories) {
				enumerated_files.InsertAll(Directory.EnumerateFiles(SystemPath.Combine(enumerated_directory, "*"), filter, recursion_depth - 1));
			}
		}
		
		FindFileHandle handle = FindFile(SystemPath.Combine(directory, "*"), file_name, file_attributes, FindFileFlags.ALL | FindFileFlags.DIRECTORIES | FindFileFlags.ARCHIVES);
		if (!handle) {
			return enumerated_files;
		}
		
		if (File.WildcardMatch(file_name, filter)) {
			enumerated_files.Insert(SystemPath.Combine(directory, file_name));
		}
		
		while (FindNextFile(handle, file_name, file_attributes)) {
			if (File.WildcardMatch(file_name, filter)) {
				enumerated_files.Insert(SystemPath.Combine(directory, file_name));
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