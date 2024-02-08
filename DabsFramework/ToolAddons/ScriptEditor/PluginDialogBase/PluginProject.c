class PluginProject: PluginDialogBase
{
	static const string SERVER_CFG = "server.cfg";
	static const string DAYZ_BIN = "dayz.bin";
	static const string GPROJ = "dayz.gproj";
	
	static const string EXECUTABLE = "DayZDiag_x64.exe";
	

	protected ref set<string> m_Prefixes = new set<string>();
	
	protected string m_ServerConfig;
	
	protected ref WorkbenchSettings m_WorkbenchSettings;
	
	void PluginProject()
	{
		string mod_root = GetModRoot();
		string repo_root = GetRepositoryRoot();

		// Load launch settings
		m_WorkbenchSettings = WorkbenchSettings.Load(mod_root + DAYZ_BIN);
		if (!m_WorkbenchSettings) {
			ErrorDialog(string.Format("Failed to load %1 in %2", DAYZ_BIN, mod_root));
			return;
		}
				
		// Load server config
		m_ServerConfig = string.Format("%1\\%2", repo_root, SERVER_CFG);
		if (!FileExist(m_ServerConfig)) {
			ErrorDialog(string.Format("Failed to load %1 in %2", SERVER_CFG, repo_root));
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
}