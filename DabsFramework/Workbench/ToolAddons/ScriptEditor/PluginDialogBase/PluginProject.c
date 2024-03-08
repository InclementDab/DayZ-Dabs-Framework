class PluginProject: PluginDialogBase
{
	static const string PROJECT_CFG = "project.cfg";
	static const string SERVER_CFG = "server.cfg";
	static const string LAUNCH_BIN = "launch.bin";
	static const string BUILD_BIN = "build.bin";
	static const string GPROJ = "dayz.gproj";
	static const string EXECUTABLE = "DayZDiag_x64.exe";
	
	protected ref map<string, string> m_ProjectSettings = new map<string, string>();
	protected ref set<string> m_Prefixes = new set<string>();
	protected string m_ServerConfig;
	
	protected ref LaunchSettings m_LaunchSettings;
	protected ref BuildSettings m_BuildSettings;
	protected ref ProjectSettings m_GprojSettings;
	
	void PluginProject()
	{
		string current_dir = GetCurrentDirectory();
		if (!LoadConfig(current_dir + PROJECT_CFG, m_ProjectSettings)) {
			ErrorDialog(string.Format("Failed to load %1 in %2", PROJECT_CFG, current_dir));
			return;
		}

		// Load launch settings
		m_LaunchSettings = LaunchSettings.Load(current_dir + LAUNCH_BIN);
		if (!m_LaunchSettings) {
			ErrorDialog(string.Format("Failed to load %1 in %2", LAUNCH_BIN, current_dir));
			return;
		}
		
		// Load build settings
		m_BuildSettings = BuildSettings.Load(current_dir + BUILD_BIN);
		if (!m_BuildSettings) {
			ErrorDialog(string.Format("Failed to load %1 in %2", BUILD_BIN, current_dir));
			return;
		}
		
		// Load server config
		m_ServerConfig = current_dir + SERVER_CFG;
		if (!FileExist(m_ServerConfig)) {
			ErrorDialog(string.Format("Failed to load %1 in %2", SERVER_CFG, current_dir));
			return;
		}
		
		// define filePatching folders into m_Prefix
		array<string> mod_splits = {};
		m_ProjectSettings["Mods"].Split(";", mod_splits);
		foreach (string mod_split: mod_splits) {
			string mod_split_edit = mod_split;
			mod_split_edit.Replace("@", ""); // more or less a hack, for the moment
			if (FileExist(string.Format("P:\\%1", mod_split_edit))) {
				m_Prefixes.Insert(mod_split_edit);
			}
		}
		
		m_Prefixes.Insert(GetPrefix());
		m_Prefixes.Insert("Scripts");
		
		m_Prefixes.Insert("DZ");

		m_Prefixes.Insert("JM");
		m_Prefixes.Insert("DabsFramework");
		m_Prefixes.Insert("HypeTrain");
		m_Prefixes.Insert("Editor");
		

		m_Prefixes.Insert("DayZExpansion");

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