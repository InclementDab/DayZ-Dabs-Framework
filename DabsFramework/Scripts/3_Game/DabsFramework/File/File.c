class File: FileSystem
{	
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
	
	static File Create(string file)
	{
		if (FileExist(file)) {
			DeleteFile(file);
		}
		
		CloseFile(OpenFile(file, FileMode.WRITE));
		return file;
	}
}

typedef string File;