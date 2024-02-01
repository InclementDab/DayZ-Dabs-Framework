class BuildSettings: SerializableBase
{
	static const int VERSION = 3;
		
	protected string m_CurrentFileLocation;
	
	//[Attribute("", "flags", "Choose which folders to build", "", EnumerateBuildableFolders())]
	int BuildFolders;
	
	[Attribute("", "editbox", "Build Command")]
	string Command;
	
	[Attribute("", "combobox", "Copy Addons", "", ParamEnumArray.FromEnum(YesNo) )]
	bool CopyAddons;
		
	[Attribute("", "editbox", "Private Key")]
	string Key;
	
	[Attribute("", "editbox", "Extra Launch Arguments")]
	string Args;
	
	[Attribute("", "combobox", "Build Dependencies", "", ParamEnumArray.FromEnum(YesNo) )]
	bool Dependencies;
	
	void Save(string file)
	{
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file, FileMode.WRITE)) {
			return;
		}
		
		Write(serializer, VERSION);
		serializer.Close();
	}
	
	static ParamEnumArray EnumerateBuildableFolders()
	{
		array<string> directories = PluginDialogBase.EnumerateDirectories("$CurrentDir:");
		if (directories.Count() > 32) {
			Workbench.Dialog("Error", "TOO MANY DIRECTORIES (> 32)");
			return {};
		}
		
		ParamEnumArray param_enums = {};
		for (int i = 0; i < directories.Count(); i++) {
			param_enums.Insert(new ParamEnum(directories[i], (Math.Pow(2, i)).ToString()));
		}
		
		return param_enums;
	}
	
	static BuildSettings Load(string file)
	{
		BuildSettings settings = new BuildSettings();
		settings.m_CurrentFileLocation = file;
		if (!FileExist(file)) {
			//settings.BuildFolders; // default all masked
			settings.Command = "pboProject";
			settings.Args = "+H";
			settings.Dependencies = true;
			settings.Save(file);
			return settings;
		}
		
		FileSerializer serializer = new FileSerializer();
		if (!serializer.Open(file, FileMode.READ)) {
			return null;
		}
		
		int version;
		if (!settings.Read(serializer, version)) {
			return null;
		}
		
		serializer.Close();
		return settings;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(VERSION);
		serializer.Write(BuildFolders);
		serializer.Write(Command);
		serializer.Write(CopyAddons);
		serializer.Write(Key);
		serializer.Write(Args);
		serializer.Write(Dependencies);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!serializer.Read(version)) {
			return false;
		}
		
		if (!serializer.Read(BuildFolders)) {
			return false;
		}		
		
		if (!serializer.Read(Command)) {
			return false;
		}
		
		if (version <= 1) {
			return true;
		}
		
		if (!serializer.Read(CopyAddons)) {
			return false;
		}
		
		if (version <= 2) {
			Args = "+H";
			Dependencies = true;
			return true;
		}
		
		if (!serializer.Read(Key)) {
			return false;
		}
		
		if (!serializer.Read(Args)) {
			return false;
		}		
		
		if (!serializer.Read(Dependencies)) {
			return false;
		}
		
		return true;
	}
	
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		Save(m_CurrentFileLocation);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}