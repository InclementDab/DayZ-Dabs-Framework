class Path
{	
#ifdef PLATFORM_WINDOWS
	const string SEPERATOR = "\\";
#else
	const string SEPERATOR = "/";
#endif

#ifdef PLATFORM_WINDOWS
	const string SEPERATOR_ALT = "/";
#else
	const string SEPERATOR_ALT = "\\";
#endif

#ifdef PLATFORM_WINDOWS
    const int MAX_SHORT_PATH = 260;
#else
    const int MAX_SHORT_PATH = 4096;
#endif

	const string VOLUME_PREFIX = "$";
	const string VOLUME_SEPERATOR = ":";
	
    static string Profile(string path = string.Empty)
    {
        return Path.Combine(string.Format("%1profile%2", Path.VOLUME_PREFIX, Path.VOLUME_SEPERATOR), path);
    }

    static string Mission(string path = string.Empty)
    {
        return Path.Combine(string.Format("%1mission%2", Path.VOLUME_PREFIX, Path.VOLUME_SEPERATOR), path);
    }

    static string Saves(string path = string.Empty)
    {
        return Path.Combine(string.Format("%1saves%2", Path.VOLUME_PREFIX, Path.VOLUME_SEPERATOR), path);
    }

    static string Storage(string path = string.Empty)
    {
        return Path.Combine(string.Format("%1storage%2", Path.VOLUME_PREFIX, Path.VOLUME_SEPERATOR), path);
    }

    static string Format(string path)
    {
        string path_copy = path;
        path_copy.Replace(Path.SEPERATOR_ALT, Path.SEPERATOR);
        path_copy.Replace("\t", string.Empty);
        path_copy.Replace("\r", string.Empty);
        path_copy.Replace("\n", string.Empty);
        return path_copy;
    }

    static bool IsPathRooted(string path)
    {
        string formatted_path = Path.Format(path);
        return (formatted_path.Length() > 0 && (formatted_path[0] == Path.VOLUME_PREFIX));
    }

    static bool IsDirectorySeperator(string ch)
    {
        return (ch.Length() == 1 && (ch == Path.SEPERATOR || ch == Path.SEPERATOR_ALT));
    }

    static string Join(notnull array<string> paths)
    {
        array<string> builder_string = {};
        int max_size;
        for (int i = 0; i < paths.Count(); i++) {
            if (paths[i].Length() == 0) {
                continue;
            }

            max_size += paths[i].Length();
            string last_char = paths[i][paths[i].Length() - 1];
            if (!Path.IsDirectorySeperator(last_char)) {
                max_size++;
            }
        }

        if (max_size > Path.MAX_SHORT_PATH) {
            ErrorEx("INTERNAL ERROR: PATH IS TOO LONG");
            return string.Empty;
        }

        for (int j = 0; j < max_size; j++) {
            if (paths[j].Length() == 0) {
                continue;
            }

            if (builder_string.Count() != 0) {
                string last_str = builder_string[j - 1];
                string last_ch = last_str[last_str.Length() - 1];
                if (!Path.IsDirectorySeperator(last_ch)) {
                    builder_string.Insert(Path.SEPERATOR);
                }
            }

            builder_string.Insert(Path.Format(paths[j]));
        }

        return string.Join(string.Empty, builder_string);
    }

    static string Join(string path0, string path1)
    {
        array<string> path_buffer = { path0, path1 };
        return Path.Join(path_buffer);
    }

    /*
        Path::Combine, similar to C# implementation
            - if available, will start at root seperator.
    */

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
                max_size += paths[i].Length();
            }

            string last_char = paths[i][paths[i].Length() - 1];
            if (!Path.IsDirectorySeperator(last_char)) {
                max_size++;
            }
        }

        if (max_size > Path.MAX_SHORT_PATH) {
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
                    builder_string.Insert(Path.SEPERATOR);
                }
            }

            builder_string.Insert(Path.Format(paths[j]));
        }

        return string.Join(string.Empty, builder_string);
	}

	static string Combine(string path0, string path1)
	{
        array<string> path_buffer = { path0, path1 };
        return Path.Combine(path_buffer);
	}

	private void Path();
	private void ~Path();
}