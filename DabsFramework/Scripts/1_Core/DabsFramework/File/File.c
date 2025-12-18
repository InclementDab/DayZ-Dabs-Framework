class File: FileSystem
{	
	static string GetName(string file)
	{
		file.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		
		array<string> path_split = {};
		file.Split(SystemPath.SEPERATOR, path_split);
		
		if (path_split.Count() == 0) {
			return string.Empty;
		}
		
		return path_split[path_split.Count() - 1];
	}
	
	static string GetNameWithoutExtension(string file)
	{
		string file_name = File.GetName(file);
		int last_period = file_name.LastIndexOf(".");
		if (last_period < 0) {
			return file_name;
		}
		
		return file_name.Substring(0, last_period);
	}
	
	static string GetExtension(string file)
	{
		for (int i = file.Length(); --i >= 0; ) {
			switch (file[i]) {
				case ".": {
					return file.Substring(i, file.Length() - i);
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
	
    static bool WildcardMatch(string input, string pattern)
    {
        int input_index, pattern_index;
        int input_wild_index = -1, pattern_wild_index = -1;

        while (input_index < input.Length()) {
            if (pattern_index < pattern.Length() && (pattern[pattern_index] == input[input_index] || pattern[pattern_index] == FileSystem.WILDCARD)) {
                if (pattern[pattern_index] == FileSystem.WILDCARD) {
                    input_wild_index = input_index;
                    pattern_wild_index = pattern_index;
                    pattern_index++;
                } else {
                    input_index++;
                    pattern_index++;
                }
            } else if (input_wild_index != -1) {
                pattern_index = pattern_wild_index + 1;
                input_index = input_wild_index + 1;
                input_wild_index++;
            } else return false;
        }

        while (pattern_index < pattern.Length() && pattern[pattern_index] == FileSystem.WILDCARD) {
            pattern_index++;
        }

        return pattern_index == pattern.Length();
    }
	
	FileHandle CreateHandle(FileMode mode)
	{
		return OpenFile(value, mode);
	}

    static array<string> ReadAllLines(string file, int max = int.MAX)
    {
        array<string> result = {};
        if (!File.Exists(file)) {
            //ErrorEx(string.Format("File not found %1", file));
            return result;
        }

        FileHandle open_file_handle = OpenFile(file, FileMode.READ);
        if (!open_file_handle) {
            ErrorEx(string.Format("File not opened %1", file));
            return result;
        }

        string line_content;
        while (FGets(open_file_handle, line_content) > 0 && result.Count() < max) {
            result.Insert(line_content);
        }

        CloseFile(open_file_handle);
        return result;
    }

    static string ReadAllText(string file)
    {
        string result;
        if (!File.Exists(file)) {
            ErrorEx(string.Format("File not found %1", file));
            return result;
        }

        FileHandle open_file_handle = OpenFile(file, FileMode.READ);
        if (!open_file_handle) {
            ErrorEx(string.Format("File not opened %1", file));
            return result;
        }

        string line_content;
        while (FGets(open_file_handle, line_content) > 0) {
            result += line_content;
            result += "\n";
        }

        CloseFile(open_file_handle);
        return result;
    }
	
	static File Create(string file)
	{
		if (FileExist(file)) {
			DeleteFile(file);
		}
		
		CloseFile(OpenFile(file, FileMode.WRITE));
		return file;
	}

    static bool Exists(string file)
    {
		// Fixes a weird bug where FileExist("") returns true
		if (file == string.Empty) {
			return false;
		}
		
        return FileExist(file);
    }
}

typedef string File;