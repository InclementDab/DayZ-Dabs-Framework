[WorkbenchPluginAttribute("Open Directory", "Opens Current Directory", "Ctrl+-", "", {"ScriptEditor"})]
class PluginOpenDirectory: PluginDialogBase
{
	void PluginOpenDirectory()
	{
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);
		string absolute_directory = GetDirectory(GetAbsolutePath(current_file));		
		absolute_directory.Replace(PATH_SEPERATOR, PATH_SEPERATOR_ALT);
		
		// rip unix bozos
		Workbench.RunCmd(string.Format("explorer %1", absolute_directory));
	}
}