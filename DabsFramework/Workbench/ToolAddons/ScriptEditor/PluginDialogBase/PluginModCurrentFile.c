/*[WorkbenchPluginAttribute("Mod File", "Mods current opened file", "Ctrl+M", "", {"ScriptEditor", "ResourceManager"})]
class PluginModCurrentFile: PluginDialogBase
{
	[Attribute("", "editbox")]
	string Prefix;
	
	protected string m_FinalFileName;
	
	void PluginModCurrentFile()
	{
	}
	
	void ~PluginModCurrentFile()
	{
	}
	
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);
		
		int line = m_ScriptEditor.GetCurrentLine();
		
		
		Print(current_file);
		Print(line);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}