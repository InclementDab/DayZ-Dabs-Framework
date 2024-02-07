class PluginProject: PluginDialogBase
{
	static const string PROJECT_CFG = "project.cfg";
	static const string SERVER_CFG = "server.cfg";
	static const string LAUNCH_BIN = "launch.bin";
	static const string BUILD_BIN = "build.bin";
	static const string GPROJ = "dayz.gproj";

//#ifdef DIAG_DEVELOPER
	static const string EXECUTABLE = "DayZDiag_x64.exe";
//#else
//	static const string EXECUTABLE = "DayZ_x64.exe";
//#endif
	
	protected ref map<string, string> m_ProjectSettings = new map<string, string>();
	protected ref set<string> m_Prefixes = new set<string>();
	protected string m_ServerConfig;
	
	protected ref LaunchSettings m_LaunchSettings;
	protected ref BuildSettings m_BuildSettings;
	protected ref ProjectSettings m_GprojSettings;
	
	void PluginProject()
	{
		string current_dir = GetModRoot();
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
		
		m_GprojSettings = ProjectSettings.Load(current_dir + GPROJ);
		foreach (ProjectSettings settings: m_GprojSettings.Classes["GameProjectClass"].Classes["Configurations"].Classes["PC"].Classes["ScriptModules"].Classes) {
			foreach (string path: settings.Classes["Paths"].Data) {
				path.TrimInPlace();
				path.Replace("\t", "");
				path.Replace("\n", "");
				path.Replace("\r", "");
				m_Prefixes.Insert(path);
			}
		}
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