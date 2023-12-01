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
		// Delete both bin files
		string current_dir = GetCurrentDirectory();
		Workbench.RunCmd(string.Format("cmd /c del %1", current_dir + PluginProject.LAUNCH_BIN));
		Workbench.RunCmd(string.Format("cmd /c del %1", current_dir + PluginProject.BUILD_BIN));
	}
	
	[ButtonAttribute("Cancel", true)]
	void Cancel()
	{
	}
}