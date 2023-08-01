[WorkbenchPluginAttribute("Close Game", "Kills all running instances of DayZ", "F10", "", {"ScriptEditor"})]
class PluginKillGame: PluginDialogBase
{
	void PluginKillGame()
	{
		Workbench.RunCmd("taskkill /F /IM DayZ_x64.exe /T");
		Workbench.RunCmd("taskkill /F /IM DayZServer_x64.exe /T");
		Workbench.RunCmd("taskkill /F /IM DayZDiag_x64.exe /T");
	}
}