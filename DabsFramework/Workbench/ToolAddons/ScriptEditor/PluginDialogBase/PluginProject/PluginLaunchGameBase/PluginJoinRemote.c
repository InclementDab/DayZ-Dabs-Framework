[WorkbenchPluginAttribute("Join Remote", "Lanches DayZ and joins remote", "Ctrl+F6", "", {"ScriptEditor", "ResourceManager"})]
class PluginJoinRemote: PluginLaunchGameBase
{
	[Attribute("", "editbox")]
	string Address;
	
	[Attribute("", "editbox")]
	int Port;
	
	[Attribute("", "editbox")]
	string Password;
	
	void PluginJoinRemote()
	{
		Address = m_LaunchSettings.JoinAddress;
		Port = m_LaunchSettings.JoinPort;
		Password = m_LaunchSettings.JoinPassword;
		
		Workbench.ScriptDialog("Join Remote", DIALOG_TAB_SIZE, this);		
	}
	
	[ButtonAttribute("Join", true)]
	void Join()
	{
		m_LaunchSettings.AutoClose = false;
		m_LaunchSettings.LaunchType = GameLaunchType.CLIENT;
		m_LaunchSettings.JoinAddress = Address;
		m_LaunchSettings.JoinPort = Port;
		m_LaunchSettings.JoinPassword = Password;
		
		m_LaunchSettings.Save(GetCurrentDirectory() + LAUNCH_BIN);
		LaunchGame(m_LaunchSettings);
	}
	
	[ButtonAttribute("Cancel")]
	void Cancel()
	{
	}
	
	override bool GetConnectionArguments(out string ip, out int port, out string password)
	{		
		ip = m_LaunchSettings.JoinAddress;
		port = m_LaunchSettings.JoinPort;
		password = m_LaunchSettings.JoinPassword;
		return true;
	}
}
