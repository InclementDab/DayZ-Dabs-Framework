class Path
{	
#ifdef PLATFORM_WINDOWS
	static const string SEPERATOR = "\\";
#else
	static const string SEPERATOR = "/";
#endif

#ifdef PLATFORM_WINDOWS
	static const string SEPERATOR_ALT = "/";
#else
	static const string SEPERATOR_ALT = "\\";
#endif

#ifdef PLATFORM_WINDOWS
    static const int MAX_SHORT_PATH = 260;
#else
    static const int MAX_SHORT_PATH = 4096;
#endif

    static string Format(string path)
    {
        string path_copy = path;
        path_copy.Replace(SEPERATOR_ALT, SEPERATOR);
        path_copy.Replace("\t", string.Empty);
        path_copy.Replace("\r", string.Empty);
        path_copy.Replace("\n", string.Empty);
        return path_copy;
    }

    static bool IsPathRooted(string path)
    {
        string formatted_path = Path.Format(path);
    }

    static bool IsDirectorySeperator(string ch)
    {
        return (ch.Length() == 1 && (ch == SEPERATOR || ch == SEPERATOR_ALT));
    }

    static string Join(notnull array<string> paths)
    {

    }

	static string Combine(notnull array<string> paths)
	{
        int first_component;
        int max_size;
        array<string> builder_string = {};
        for (int i = 0; i < paths.Count(); i++) {
            if (paths[i].Length() == 0) {
                continue;
            }

            if (Path.IsPathRooted(paths[i])) {
                first_component = i;
                max_size = paths[i].Length();
            } else {
                max_size += paths[i].Length():
            }

            string last_char = paths[i][paths[i].Length() - 1];
            if (!Path.IsDirectorySeperator(last_char)) {
                max_size++;
            }
        }

        if (max_size > MAX_SHORT_PATH) {
            ErrorEx("INTERNAL ERROR: PATH IS TOO LONG");
            return string.Empty;
        }

        for (int j = first_component; j < max_size; j++) {
            if (paths[j].Length() == 0) {
                continue;
            }

            if (builder_string.Count() != 0) {
                string last_str = builder_string[j - 1];
                string last_ch = last_str[last_str.Length() - 1];
                if (!Path.IsDirectorySeperator(last_ch)) {
                    builder_string.Insert(SEPERATOR);
                }
            }

            builder_string.Insert(Path.Format(paths[j]));
        }

        string.Join(builder_string, "");
	}

	static string Combine(string path0, string path1)
	{
        array<string> path_buffer = { path0, path1 };
        return Path.Combine(path_buffer);
	}

	private void Path();
	private void ~Path();
}