[WorkbenchPluginAttribute("Launch Game", "Lanches DayZ", "F6", "", {"ScriptEditor", "ResourceManager"})]
class PluginLaunchGame: PluginLaunchGameBase
{
	void PluginLaunchGame()
	{
		KillTask(EXECUTABLE);
		LaunchGame(m_WorkbenchSettings);
	}
	
	override bool GetConnectionArguments(out string ip, out int port, out string password)
	{
		if (m_WorkbenchSettings.LaunchType != GameLaunchType.BOTH) {
			return false;
		}
		
		ip = "127.0.0.1";
		port = 2302;
		password = m_WorkbenchSettings.ServerPassword;
		return true;
	}
}