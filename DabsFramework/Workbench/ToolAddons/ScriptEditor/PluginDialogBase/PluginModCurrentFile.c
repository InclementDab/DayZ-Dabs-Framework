//[WorkbenchPluginAttribute("Mod File", "Mods current opened file", "Ctrl+M", "", {"ScriptEditor"})]
class PluginModCurrentFile: PluginDialogBase
{
	protected string m_CurrentFile;
	protected bool m_IsModded;
	protected int m_ScriptModule;
	
	[Attribute("", "editbox")]
	string Name;
	
	[Attribute("", "editbox")]
	string Prefix;
	
	void PluginModCurrentFile()
	{
		if (!m_ScriptEditor.GetCurrentFile(m_CurrentFile)) {
			return; // Doesnt need an error, really
		}

		if (!GetClassFromFileAndCursorPosition(m_CurrentFile, m_ScriptEditor.GetCurrentLine(), Name, m_IsModded)) {
			return; // Couldnt find a class to mod
		}
		
		m_ScriptModule = GetScriptModuleFromFile(m_CurrentFile);
	}
	
	// Remember to call ScriptDialog in Run to use the built in sticky settings
	override void Run()
	{		
		Workbench.ScriptDialog("New Modded Script", DIALOG_TAB_SIZE, this);
	}
		
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		Print(m_CurrentFile);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}