[WorkbenchPluginAttribute("Join Remote", "Lanches DayZ and joins remote", "F5", "", {"ScriptEditor", "ResourceManager"})]
class PluginJoinRemote: PluginLaunchGameBase
{
	[Attribute("", "editbox")]
	string Address;
	
	[Attribute("", "editbox")]
	int Port = 2302;
	
	[Attribute("", "editbox")]
	string Password;
	
	void PluginJoinRemote()
	{
		Address = m_WorkbenchSettings.LastUsedAddress;
		Workbench.ScriptDialog("Join Remote", DIALOG_TAB_SIZE, this);
	}
	
	[ButtonAttribute("Join", true)]
	void Join()
	{
		m_WorkbenchSettings.LaunchType = GameLaunchType.CLIENT;
		m_WorkbenchSettings.LastUsedAddress = Address;
		m_WorkbenchSettings.Save();
		LaunchGame(m_WorkbenchSettings);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
	
	override bool GetConnectionArguments(out string ip, out int port, out string password)
	{
		ip = Address;
		port = Port;
		password = Password;
		return true;
	}
}