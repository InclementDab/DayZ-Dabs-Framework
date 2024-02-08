[WorkbenchPluginAttribute("Open Directory", "Opens Current Directory", "Ctrl+-", "", {"ScriptEditor"})]
class PluginOpenDirectory: PluginProject
{
	void PluginOpenDirectory()
	{
		string current_file;
		m_ScriptEditor.GetCurrentFile(current_file);
		string absolute_directory = GetDirectory(GetAbsolutePath(current_file));
		
#ifdef PLATFORM_WINDOWS
		Workbench.RunCmd(string.Format("explorer %1", absolute_directory));
#else 
		Workbench.RunCmd(string.Format("xdg-open %1", absolute_directory));
#endif		
	}
}