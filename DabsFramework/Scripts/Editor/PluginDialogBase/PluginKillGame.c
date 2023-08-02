[WorkbenchPluginAttribute("Close Game", "Kills all running instances of DayZ", "F10", "", {"ScriptEditor"})]
class PluginKillGame: PluginProject
{
	void PluginKillGame()
	{
		KillTask(EXECUTABLE);
	}
}