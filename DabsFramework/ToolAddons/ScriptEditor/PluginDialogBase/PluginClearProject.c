[WorkbenchPluginAttribute("Clear All Settings", "Deletes Local Project Settings", "", "", {"ScriptEditor", "ResourceManager"})]
class PluginClearProject: PluginDialogBase
{
	override void Run()
	{
		Workbench.ScriptDialog("Clear Project", "Are you sure you want to clear all settings? This cannot be undone.", this);
	}
	
	[ButtonAttribute("Clear")]
	void Clear()
	{
		string current_dir = GetModRoot();
		Workbench.RunCmd(string.Format("cmd /c del %1\\%2", current_dir, PluginProject.DAYZ_BIN));
	}
	
	[ButtonAttribute("Cancel", true)]
	void Cancel()
	{
	}
}