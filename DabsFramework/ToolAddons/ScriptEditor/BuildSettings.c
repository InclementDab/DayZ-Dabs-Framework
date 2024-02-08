class BuildSettings: SerializableBase
{
	static const int VERSION = 3;
		
	protected string m_CurrentFileLocation;
	
	//[Attribute("", "flags", "Choose which folders to build", "", EnumerateBuildableFolders())]
	int BuildFolders;
	

	
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
		
	}
	
	override bool Read(Serializer serializer, int version)
	{
		
		
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