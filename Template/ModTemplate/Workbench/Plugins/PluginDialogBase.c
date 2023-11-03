class PluginDialogBase: WorkbenchPlugin
{	
	// Sizes the dialog to max size without putting a scroll bar on the bottom
	static const string DIALOG_TAB_SIZE = "\t\t\t\t\t\t\t\t\t";
	static const string PATH_SEPERATOR = "\\";
	static const string PATH_SEPERATOR_ALT = "/";
	static const string DEFAULT_EXTENSION = ".c";
	static const ref array<string> LOG_FILE_TYPES = {".log", ".rpt", ".adm", ".mdmp"};
	static const ref array<string> WB_DIR_DEFAULTS = {"Addons", "bliss", "dta", "platforms"};
	
	protected ScriptEditor m_ScriptEditor = Workbench.GetModule("ScriptEditor");
	protected ResourceBrowser m_ResourceBrowser = Workbench.GetModule("ResourceBrowser");
	
	void ErrorDialog(string error)
	{
		Workbench.Dialog(string.Format("Error: %1", Type()), error);
	}
	
	static string GetPrefix()
	{
		array<string> current_dir_split = {};
		GetCurrentDirectory().Split("/", current_dir_split);
		return current_dir_split[current_dir_split.Count() - 2];
	}
	
	static string GetSourceDataDirectory()
	{
		string abs;
		Workbench.GetAbsolutePath("$SourceData:", abs);
		return abs;
	}
	
	static string GetCurrentDirectory()
	{
		string abs;
		Workbench.GetAbsolutePath("$CurrentDir:", abs);
		return abs;
	}
	
	static string GetWorkbenchDirectory()
	{
		string workbench_dir;
		Workbench.GetCwd(workbench_dir);
		return workbench_dir;
	}
	
	static string GetRootDirectory()
	{
		string root_dir;
		Workbench.GetAbsolutePath(string.Empty, root_dir);
		return root_dir;
	}
	
	static string GetAbsolutePath(string path)
	{
		string absolute_path;
		Workbench.GetAbsolutePath(path, absolute_path);
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
		string directory = GetDirectory(path);
		array<string> child_directories = {};
		string file_name;
		FileAttr file_attributes;
		FindFileHandle handle = FindFile(directory + PATH_SEPERATOR + "*", file_name, file_attributes, FindFileFlags.DIRECTORIES);
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
}