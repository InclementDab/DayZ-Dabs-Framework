[WorkbenchPluginAttribute("Open In Code", "Opens Current File in Code", "Ctrl+=", "", {"ScriptEditor"})]
class PluginOpenInCode: PluginDialogBase
{
	void PluginOpenInCode()
	{
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);
		string absolute_directory = GetAbsolutePath(current_file);
		absolute_directory.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		
		int current_line = m_ScriptEditor.GetCurrentLine() + 1;
		if (Workbench.RunCmd(string.Format("cmd /c \"code --goto %1:%2:1\"", absolute_directory, current_line))) {
			ErrorDialog("Failed to open vscode");
		}
	}
}