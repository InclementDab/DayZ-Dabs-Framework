//[WorkbenchPluginAttribute("Mod File", "Mods current opened file", "Ctrl+M", "", {"ScriptEditor"})]
class PluginModCurrentFile: PluginDialogBase
{	
	protected string m_CurrentFile, m_ClassName, m_FinalFileName;
	protected bool m_IsModded;
	protected int m_ScriptModule = -1;
	
	[Attribute("", "editbox")]
	string Folder;
	
	[Attribute("", "editbox")]
	string FileName;	
	
	[Attribute("", "editbox")]
	string Prefix;
		
	void PluginModCurrentFile()
	{		
		if (!m_ScriptEditor.GetCurrentFile(m_CurrentFile)) {
			return; // Doesnt need an error, really
		}

		if (!GetClassFromFileAndCursorPosition(m_CurrentFile, m_ScriptEditor.GetCurrentLine(), m_ClassName, m_IsModded)) {		
			PrintFormat("Couldnt find classname in file %1", m_CurrentFile);	
			return; // Couldnt find a class to mod
		}
		
		m_ScriptModule = GetScriptModuleFromFile(m_CurrentFile);
		
		Prefix = GetPrefix();
		Workbench.ScriptDialog("New Modded Script", DIALOG_TAB_SIZE, this);
		//Workbench.SearchResources(m_ClassName, OnResourceFound);
	}
	
	void ~PluginModCurrentFile()
	{
		if (m_FinalFileName != string.Empty) {
			m_ScriptEditor.SetOpenedResource(m_FinalFileName);
			return;
		}
		
		if (m_ClassName != string.Empty && m_IsModded) {
			Print(string.Format("Could not find file with non-modded class %1, is the file name the same as your class?", m_ClassName));
		}
	}
			
	protected void OnResourceFound(string resource)
	{
		if (Folder != string.Empty) {
			return;
		}
		
		string class_name;
		bool is_modded;
		if (!GetClassFromFileAndCursorPosition(resource, 0, class_name, is_modded)) {			
			return;
		}
		
		if (is_modded) {
			return;
		}
						
		array<string> split = {};
		resource.Split(":", split);
		Print(resource);
		Folder = GetPrefix();
		Print(Folder);
				
		array<string> path_split = {};
		split[1].Split(PATH_SEPERATOR_ALT, path_split);
		for (int i = 1; i < path_split.Count() - 1; i++) {
			Folder += PATH_SEPERATOR + path_split[i];
		}
		
		FileName = path_split[path_split.Count() - 1];
		Workbench.ScriptDialog("New Modded Script", DIALOG_TAB_SIZE, this);
	}
	
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		//if (File.Length() == 0) {
		//	return;
		//}
		
		// Appending the extension
		//array<string> file_split = {};
		//File.Split(".", file_split);
		//if (file_split.Count() < 2) {
		//	File = file_split[0] + DEFAULT_EXTENSION;
		//}
		
		string absolute_file = GetCurrentScriptModulePath(m_ScriptModule) + StripScriptModuleFromPath(m_CurrentFile);	
		Print(absolute_file);
		//Print(GetCurrentScriptModulePath(m_ScriptModule));
		//Print(m_CurrentFile);
		//Print(absolute_file);
		
		return;
		if (FileExist(absolute_file)) {
			ErrorDialog(string.Format("File %1 already exists!", FileName));
			return;
		}
		
		FileHandle file_handle = CreateFile(absolute_file);
		if (!file_handle) {
			return;
		}
		
		FPrint(file_handle, string.Format("modded class %1\n{\n}", m_ClassName));
		CloseFile(file_handle);
		
		m_FinalFileName = Folder + PATH_SEPERATOR + FileName;
		Print(m_FinalFileName);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}