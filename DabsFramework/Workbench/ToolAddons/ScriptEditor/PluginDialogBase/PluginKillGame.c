[WorkbenchPluginAttribute("Close Game", "Kills all running instances of DayZ", "F10", "", {"ScriptEditor", "ResourceManager"})]
class PluginKillGame: PluginDialogBase
{
	void PluginKillGame()
	{
		string current_dir = GetCurrentDirectory();
		LaunchSettings launch_settings = LaunchSettings.Load(current_dir + "launch.bin");
		if (!launch_settings) {
			ErrorDialog(string.Format("Failed to load %1 in %2", "launch.bin", current_dir));
			return;
		}
		
		Workbench.RunCmd(string.Format("taskkill /f /im %1 /t", launch_settings.GetExecutableName()));
	}
}