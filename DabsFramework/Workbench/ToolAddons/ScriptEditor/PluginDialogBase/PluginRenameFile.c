[WorkbenchPluginAttribute("Rename File", "Renames current file", "Ctrl+Shift+F2", "", {"ScriptEditor"})]
class PluginRenameFile: PluginDialogBase
{
	[Attribute("", "editbox", "New file name")]
	string FileName;
	
	protected string m_FinalFileName;
	
	void PluginRenameFile()
	{
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);
		string absolute_file = GetAbsolutePath(current_file);
		string file_name = GetFile(absolute_file);
		
		FileName = file_name;
		
		Workbench.ScriptDialog("Rename File", DIALOG_TAB_SIZE, this);
	}
	
	void ~PluginRenameFile()
	{
		if (m_FinalFileName != string.Empty) {
			m_ScriptEditor.SetOpenedResource(m_FinalFileName);
		}
	}
	
	[ButtonAttribute("Ok", true)]
	void Ok()
	{
		// Appending the extension
		array<string> file_split = {};
		FileName.Split(".", file_split);
		if (file_split.Count() < 2) {
			FileName = file_split[0] + DEFAULT_EXTENSION;
		}
		
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);
		string absolute_file = GetAbsolutePath(current_file);
		
		m_FinalFileName = GetDirectory(absolute_file) + Path.SEPERATOR + FileName;
		if (!CopyFile(absolute_file, m_FinalFileName)) {
			ErrorDialog("Failed to rename file");
			return;
		}
		
		DeleteFile(absolute_file);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
}