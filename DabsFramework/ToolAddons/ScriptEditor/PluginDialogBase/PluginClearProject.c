[WorkbenchPluginAttribute("Clear All Settings", "Deletes Local Project Settings", "", "", {"ScriptEditor", "ResourceManager"})]
class PluginClearProject: PluginProject
{
	override void Run()
	{
		Workbench.ScriptDialog("Clear Project", "Are you sure you want to clear all settings? This cannot be undone.", this);
	}
	
	[ButtonAttribute("Clear")]
	void Clear()
	{
		DeleteFile(string.Format("%1\\%2", m_Root, PluginProject.DAYZ_BIN));
	}
	
	[ButtonAttribute("Cancel", true)]
	void Cancel()
	{
	}
}