[WorkbenchPluginAttribute("Close Game", "Kills all running instances of DayZ", "F10", "", {"ScriptEditor", "ResourceManager"})]
class PluginKillGame: PluginProject
{
	void PluginKillGame()
	{
		KillTask(EXECUTABLE);
		
		string x;
		Workbench.GetAbsolutePath("$CurrentDir:", x);
		Print(x);
	}
}