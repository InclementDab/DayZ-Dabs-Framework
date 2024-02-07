[WorkbenchPluginAttribute("Find Unmodded File", "Searches for the unmodded version of your class", "Ctrl+B", "", {"ScriptEditor"})]
class PluginNavigateToUnmoddedFile: PluginDialogBase
{
	protected string m_CurrentFile, m_ClassName, m_FinalFileName;
	protected bool m_IsModded;
	protected int m_ScriptModule = -1;
	
	void PluginNavigateToUnmoddedFile()
	{
		if (!m_ScriptEditor.GetCurrentFile(m_CurrentFile)) {
			return; // Doesnt need an error, really
		}

		if (!GetClassFromFileAndCursorPosition(m_CurrentFile, m_ScriptEditor.GetCurrentLine(), m_ClassName, m_IsModded)) {		
			PrintFormat("Couldnt find classname in file %1", m_CurrentFile);	
			return; // Couldnt find a class to mod
		}
		
		m_ScriptModule = GetScriptModuleFromFile(m_CurrentFile);
	}
	
	void ~PluginNavigateToUnmoddedFile()
	{
		if (m_FinalFileName != string.Empty) {
			m_ScriptEditor.SetOpenedResource(m_FinalFileName);
			return;
		}
		
		if (m_ClassName != string.Empty && m_IsModded) {
			Print(string.Format("Could not find file with non-modded class %1, is the file name the same as your class?", m_ClassName));
		}
	}
	
	override void Run()
	{
		if (m_ScriptModule == -1 || !m_IsModded) {			
			return;
		}
		
		Workbench.SearchResources(m_ClassName, OnResourceFound);
	}
	
	protected void OnResourceFound(string resource)
	{
		if (m_FinalFileName != string.Empty) {
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
		m_FinalFileName = split[1];
	}
}