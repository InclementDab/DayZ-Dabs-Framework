[WorkbenchPluginAttribute("Delete File", "Deletes Current File", "Ctrl+Shift+Delete", "", {"ScriptEditor"})]
class PluginDeleteFile: PluginDialogBase
{
	protected string m_CurrentFile;
	
	void PluginDeleteFile() 
	{	
		string current_file_relative;
		if (!m_ScriptEditor.GetCurrentFile(current_file_relative)) {
			Error("Failed to acquire current file");
			return;
		}
		
		if (!Workbench.GetAbsolutePath(current_file_relative, m_CurrentFile)) {
			Error("Failed to acquire absolute path");
			return;
		}

		array<string> current_file_split = {};
		m_CurrentFile.Split("/", current_file_split);
		string current_file_name = current_file_split[current_file_split.Count() - 1];
		Workbench.ScriptDialog("Delete File", current_file_name, this);
	}
	
	[ButtonAttribute("OK", true)]
	void Ok()
	{
		string current_file = m_CurrentFile;
		current_file.Replace(Path.SEPERATOR_ALT, Path.SEPERATOR);
		Workbench.RunCmd(string.Format("cmd /c del %1", current_file));
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}