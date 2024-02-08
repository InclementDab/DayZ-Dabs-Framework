class PluginProject: WorkbenchPlugin
{		
	// Sizes the dialog to max size without putting a scroll bar on the bottom
	static const string DIALOG_TAB_SIZE = "\t\t\t\t\t\t\t\t\t";
	static const string PATH_SEPERATOR = "\\";
	static const string PATH_SEPERATOR_ALT = "/";
	static const string DEFAULT_EXTENSION = ".c";
	static const string SERVER_CFG = "server.cfg";
	static const string DAYZ_BIN = "dayz.bin";
	static const string GPROJ = "dayz.gproj";
	static const string EXECUTABLE = "DayZDiag_x64.exe";
	static const ref array<string> LOG_FILE_TYPES = {".log", ".rpt", ".adm", ".mdmp"};
	static const ref array<string> WB_DIR_DEFAULTS = {"Addons", "bliss", "dta", "platforms"};
	static const ref array<string> SCRIPT_MODULES = {"1_core", "2_gamelib", "3_game", "4_world", "5_mission", "workbench"};
	
	protected ScriptEditor m_ScriptEditor = Workbench.GetModule("ScriptEditor");
	protected ResourceBrowser m_ResourceBrowser = Workbench.GetModule("ResourceManager");
	
	protected ref WorkbenchSettings m_WorkbenchSettings;
	protected ref set<string> m_Prefixes = new set<string>();
	
	protected string m_Root = GetAbsolutePath("$CurrentDir:");
	protected string m_Workdrive = GetAbsolutePath("$Workdrive:");
	protected string m_GameDirectory = GetAbsolutePath("$SourceData:");
	protected string m_AppDirectory = GetAbsolutePath("$App:");
	protected string m_Repository;
		
	void PluginProject()
	{
		// Load launch settings
		m_WorkbenchSettings = WorkbenchSettings.Load(m_Root + DAYZ_BIN);
		if (!m_WorkbenchSettings) {
			ErrorDialog(string.Format("Fatal, Failed to load %1 in %2", DAYZ_BIN, m_Root));
			return;
		}
		
		if (m_Workdrive == string.Empty) {
			ErrorDialog(string.Format("Fatal, No $Workdrive entry"));
			return;
		}
		
		if (m_GameDirectory == string.Empty) {
			ErrorDialog(string.Format("Fatal, No $SourceData entry"));
			return;
		}
		
		if (!FileExist(string.Format("%1\\%2", m_GameDirectory, EXECUTABLE))) {
			ErrorDialog(string.Format("Fatal, Failed to find %1 in %2", EXECUTABLE, m_GameDirectory));
			return;
		}
						
		if (!FileExist(string.Format("%1\\%2", m_AppDirectory, "workbenchApp.exe"))) {
			ErrorDialog(string.Format("Fatal, Invalid App Directory: %1", m_AppDirectory));
			return;
		}
		
		if (!GetCLIParam("repository", m_Repository) || m_Repository == string.Empty) {
			ErrorDialog(string.Format("Fatal, -repository was empty or not specified in the launch parameters."));
			return;
		}
				
		string mod_list;
		if (GetCLIParam("mod", mod_list)) {
			array<string> mod_splits = {};
			mod_list.Split(";", mod_splits);
			for (int i = 0; i < mod_splits.Count(); i++) {
				Print(mod_splits[i]);
			}
		}
		
		
		// define filePatching folders into m_Prefix
		/*array<string> mod_splits = {};
		m_ProjectSettings["Mods"].Split(";", mod_splits);
		foreach (string mod_split: mod_splits) {
			string mod_split_edit = mod_split;
			mod_split_edit.Replace("@", ""); // more or less a hack, for the moment
			if (FileExist(string.Format("P:\\%1", mod_split_edit))) {
				m_Prefixes.Insert(mod_split_edit);
			}
		}
		
		m_Prefixes.Insert(GetModPrefix());*/
	}
	
	protected void ErrorDialog(string error)
	{
		Workbench.Dialog(string.Format("Error: %1", Type()), error);
	}
	
	protected bool LoadConfig(string file, inout map<string, string> settings)
	{
		if (!FileExist(file)) {
			return false;
		}

		FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			return false;
		}
		
		string data, buffer;
		while (ReadFile(handle, buffer, 256) != 0) {
		    data += buffer;
		}
		
		CloseFile(handle);
		
		array<string> lines = {};
		data.Split("\n", lines);
		
		foreach (string line: lines) {
			line.TrimInPlace();
			line.Replace("\t", "");
			line.Replace("\n", "");
			line.Replace("\r", "");
			if (line == string.Empty) {
				continue;
			}
			
			string key = string.Empty;
			string token = string.Empty;
			
			// search forward for first equals sign			
			while (line.ParseStringEx(token) > 1) {
				key += token;
			}
			
			settings[key] = line;
		}
		
		return true;
	}
	
	protected bool LoadPrefixes(string file, inout array<string> lines)
	{
	    FileHandle handle = OpenFile(file, FileMode.READ);
		if (!handle) {
			return false;
		}
	
	    string data, buffer;
	    while (ReadFile(handle, buffer, 256) != 0) {
	        data += buffer;
		}
	
	    data.Split("\n", lines);
	    CloseFile(handle);
		return true;
	}
	
	protected string GetModPrefix()
	{
		array<string> split = {};
		m_Root.Split(PATH_SEPERATOR, split);
		return split[split.Count() - 1];
	}
	
	static string GetAbsolutePath(string path)
	{
		string absolute_path;
		Workbench.GetAbsolutePath(path, absolute_path);
		absolute_path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		return absolute_path;
	}
						
	static string GetRelativePath(string root_path, string full_path)
	{
		root_path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		full_path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		
		array<string> root_path_split = {};
		array<string> full_path_split = {};
		
		root_path.Split(PATH_SEPERATOR, root_path_split);
		full_path.Split(PATH_SEPERATOR, full_path_split);
		
		string result;
		for (int i = 0; i < full_path_split.Count(); i++) {
			if (i > root_path_split.Count() - 1 || full_path_split[i] != root_path_split[i]) {
				result += full_path_split[i];
				if (i != full_path_split.Count() - 1) {
					result += PATH_SEPERATOR;
				}
			}
		}
		
		return result;
	}
	
	static string GetDirectory(string path)
	{		
		path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		
		array<string> path_split = {};
		path.Split(PATH_SEPERATOR, path_split);
		string directory;
		for (int i = 0; i < path_split.Count() - 1; i++) {
			if (path_split[i].Contains(".")) {
				// pop upon finding file. Substring because we added a seperator to the end last loop
				return directory;
			}
			
			directory += path_split[i];
			if (i != path_split.Count() - 1) {
				directory += PATH_SEPERATOR;
			}
		}
		
		return directory;
	}
	
	static string GetFile(string path)
	{
		path.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		
		array<string> path_split = {};
		path.Split(PATH_SEPERATOR, path_split);
		
		if (path_split.Count() == 0) {
			return string.Empty;
		}
		
		return path_split[path_split.Count() - 1];
	}
	
	static array<string> EnumerateDirectories(string path)
	{
		array<string> child_directories = {};
		string file_name;
		FileAttr file_attributes;
		FindFileHandle handle = FindFile(path + PATH_SEPERATOR + "*", file_name, file_attributes, FindFileFlags.DIRECTORIES);
		if (!handle) {
			return child_directories;
		}
		
		if (file_attributes == FileAttr.DIRECTORY) {
			child_directories.Insert(file_name);
		}
		
		while (FindNextFile(handle, file_name, file_attributes)) {
			if (file_attributes == FileAttr.DIRECTORY) {
				child_directories.Insert(file_name);
			}
		}
		
		CloseFindFile(handle);
		return child_directories;
	}
	
	static FileHandle CreateFile(string file)
	{		
		// Creates needed directories
		string absolute_file_rebuild;
		array<string> absolute_file_split = {};
		file.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);
		file.Split(PATH_SEPERATOR, absolute_file_split);
		for (int i = 0; i < absolute_file_split.Count(); i++) {
			if (absolute_file_split[i].Contains(".")) {
				break;
			}
			
			absolute_file_rebuild += absolute_file_split[i];
													// drive specifier
			if (!FileExist(absolute_file_rebuild) && !absolute_file_split[i].Contains(":")) {
				Workbench.RunCmd("cmd /c mkdir " + absolute_file_rebuild);
			}
			
			absolute_file_rebuild += PATH_SEPERATOR_ALT;
		}
		
		// Create the file
		int result = Workbench.RunCmd(string.Format("cmd /c copy NUL \"%1\"", file), true);
		if (result != 0) {
			return null;
		}
				
		return OpenFile(file, FileMode.WRITE);
	}
		
	static int RunCommandPrompt(string cmd, bool wait = false)
	{
		return Workbench.RunCmd(string.Format("cmd /c %1", cmd), wait);
	}
	
	static int PromiseSymLink(string source, string target)
	{
		if (!FileExist(target)) {
			return RunCommandPrompt(string.Format("mklink /j \"%2\" \"%1\"", source, target), true);
		}
		
		return 0;
	}
	
	static void KillTask(string task_name)
	{
		Workbench.RunCmd(string.Format("taskkill /F /IM %1 /T", task_name), true);
	}
	
	static void CleanLogFolder(string folder)
	{
		string name;
		FileAttr attribute;
		array<string> files = {};

		FindFileHandle handle = FindFile(folder + PATH_SEPERATOR + "*", name, attribute, 0);
		if (handle) {
			if (name.Length() > 0 && !(attribute & FileAttr.DIRECTORY)) {
				files.Insert(name);
			}

			while (FindNextFile(handle, name, attribute)) {
				if (name.Length() > 0 && !(attribute & FileAttr.DIRECTORY)) {
					files.Insert(name);
				}
			}
		}

		CloseFindFile(handle);
		
		map<string, int> amount_found = new map<string, int>();
		foreach (string file: files) {
			array<string> file_split = {};
			file.Split(".", file_split);
			if (file_split.Count() < 2) {
				continue;
			}
			
			string extension = file_split[1];
			amount_found[extension] = amount_found[extension] + 1;
			if (amount_found[extension] > 5) {
				DeleteFile(folder + PATH_SEPERATOR_ALT + file);
			}
		}
	}
	
	static void CopyFiles(string source, string destination)
	{	
		string filename;
		FileAttr fileattr;
		FindFileHandle hdnl = FindFile(source + PATH_SEPERATOR + "*", filename, fileattr, FindFileFlags.ALL);
		if (fileattr == FileAttr.DIRECTORY) {
			MakeDirectory(destination + PATH_SEPERATOR + filename);
			CopyFiles(source + PATH_SEPERATOR + filename, destination + PATH_SEPERATOR + filename);
		} else {
			CopyFile(source + PATH_SEPERATOR + filename, destination + PATH_SEPERATOR + filename);
		}
		
		while (FindNextFile(hdnl, filename, fileattr)) {
			if (fileattr == FileAttr.DIRECTORY) {
				MakeDirectory(destination + PATH_SEPERATOR + filename);
				CopyFiles(source + PATH_SEPERATOR + filename, destination + PATH_SEPERATOR + filename);
			} else {
				CopyFile(source + PATH_SEPERATOR + filename, destination + PATH_SEPERATOR + filename);
			}
		}
		
		CloseFindFile(hdnl);
	}
	
	static int GetScriptModuleFromFile(string file)
	{
		file.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);	
		file.ToLower();
		
		array<string> tokens = {};
		file.Split(PATH_SEPERATOR, tokens);
		foreach (string token: tokens) {
			int search_result = SCRIPT_MODULES.Find(token);
			if (search_result != -1) {
				return search_result;	
			}
		}
		
		return -1;
	}
	
	static string StripScriptModuleFromPath(string file)
	{				
		file.Replace(PATH_SEPERATOR_ALT, PATH_SEPERATOR);	
		file.ToLower();
		
		string result;
		array<string> tokens = {};
		file.Split(PATH_SEPERATOR, tokens);
		for (int i = tokens.Count() - 1; i >= 0; i--) {
			if (SCRIPT_MODULES.Find(tokens[i]) != -1) {
				return result;
			}
			
			result = PATH_SEPERATOR + tokens[i] + result;
		}
		
		return result;
	}
	
 	static bool GetClassFromFileAndCursorPosition(string current_file, int current_line, out string class_name, out bool is_modded)
	{
		FileHandle current_file_handle = OpenFile(current_file, FileMode.READ);
		if (!current_file_handle) {
			return false;
		}
		
		array<string> file_contents = {};
		string data, buffer;
	    while (ReadFile(current_file_handle, buffer, 256) != 0) {
	        data += buffer;
		}
	
	    data.Split("\n", file_contents);
	    CloseFile(current_file_handle);
		
		// Bounding check
		current_line = Math.Min(current_line, file_contents.Count() - 1);
		string token, line;
		
		// First, search UP /\
		for (int i = current_line; i >= 0; i--) {			
			line = file_contents[i];
			if (!line.ParseStringEx(token)) {
				continue;
			}
			
			if (token == "modded") {
				line.ParseStringEx(token);
				is_modded = true;
			}
			
			if (token == "class" && line.ParseStringEx(class_name)) {
				return true;
			}
			
			is_modded = false;
		}
		
		// Second, search DOWN \/
		for (int j = current_line; j < file_contents.Count(); j++) {
			line = file_contents[j];
			if (!line.ParseStringEx(token)) {
				continue;
			}
			
			if (token == "modded") {
				line.ParseStringEx(token);
				is_modded = true;
			}
			
			if (token == "class" && line.ParseStringEx(class_name)) {
				return true;
			}
			
			is_modded = false;
		}
		
		return false;
	}
}