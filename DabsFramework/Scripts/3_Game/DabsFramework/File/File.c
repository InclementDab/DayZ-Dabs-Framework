class Directory: Managed
{
	static const string PATH_SEPERATOR = "\\";
	static const string PATH_SEPERATOR_ALT = "/";
	
	static const string PROFILES = "$profiles:";
	
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

class FileStream: Managed
{
	
}

typedef string File;
class File: string
{
	static string GetExtension(string file)
	{
		array<string> file_split = {};
		file.Split(".", file_split);
		if (file_split.Count() == 1) {
			return string.Empty;
		}
		
		return file_split[file_split.Count() - 1];
	}
	
    static bool WildcardMatch(string input, string pattern)
    {
        int inputIndex = 0;
        int patternIndex = 0;
        int inputStarIndex = -1;
        int patternStarIndex = -1;

        while (inputIndex < input.Length()) {
            if (patternIndex < pattern.Length() && (pattern[patternIndex] == input[inputIndex] || pattern[patternIndex] == "*")) {
                if (pattern[patternIndex] == "*") {
                    inputStarIndex = inputIndex;
                    patternStarIndex = patternIndex;
                    patternIndex++;
                } else {
                    inputIndex++;
                    patternIndex++;
                }
            } else if (inputStarIndex != -1) {
                patternIndex = patternStarIndex + 1;
                inputIndex = inputStarIndex + 1;
                inputStarIndex++;
            } else {
                return false;
            }
        }

        while (patternIndex < pattern.Length() && pattern[patternIndex] == "*") {
            patternIndex++;
        }

        return patternIndex == pattern.Length();
    }
	
	FileHandle CreateHandle(FileMode mode)
	{
		return OpenFile(value, mode);
	}
	
	static FileStream Open()
	{
		
	}
	
	static bool Exists(File file)
	{
		return FileExist(file);
	}
	
	static File Create(string file)
	{
		if (FileExist(file)) {
			DeleteFile(file);
		}
		
		CloseFile(OpenFile(file, FileMode.WRITE));
		return file;
	}
}