[WorkbenchPluginAttribute("Close Game", "Kills all running instances of DayZ", "F10", "", {"ScriptEditor"})]
class PluginKillGame: PluginDialogBase
{
	void PluginKillGame()
	{
		KillTask("DayZ_x64.exe");
		KillTask("DayZServer_x64.exe");
		KillTask("DayZDiag_x64.exe");
	}
}