//[WorkbenchPluginAttribute("Find Unmodded File", "Searches for the unmodded version of your class", "Ctrl+B", "", {"ScriptEditor"})]
class PluginNavigateToUnmoddedFile: PluginDialogBase
{
	protected string m_CurrentFile, m_ClassName, m_FinalFileName;
	protected bool m_IsModded;
	protected int m_ScriptModule;
	
	void PluginNavigateToUnmoddedFile()
	{
		if (!m_ScriptEditor.GetCurrentFile(m_CurrentFile)) {
			return; // Doesnt need an error, really
		}

		if (!GetClassFromFileAndCursorPosition(m_CurrentFile, m_ScriptEditor.GetCurrentLine(), m_ClassName, m_IsModded)) {			
			return; // Couldnt find a class to mod
		}
		
		if (!m_IsModded) {
			PrintFormat("Class %1 isn't modded", m_ClassName);
			return;
		}
		
		m_ScriptModule = GetScriptModuleFromFile(m_CurrentFile);
	}
	
	void ~PluginNavigateToUnmoddedFile()
	{
		if (m_FinalFileName != string.Empty) {
			m_ScriptEditor.SetOpenedResource(m_FinalFileName);
		}
	}
	
	override void Run()
	{
		if (m_ScriptModule != -1) {
			
		}
	}
}