[WorkbenchPluginAttribute("Configure Project", "Configure DayZ Project", "Ctrl+Shift+F6", "", {"ScriptEditor", "ResourceManager"})]
class PluginConfigureProject: PluginProject
{
	override void Run()
	{
		super.Run();
		
		Workbench.ScriptDialog("Edit Environment", DIALOG_TAB_SIZE, m_LaunchSettings);
	}
	
	override void Configure()
	{
		super.Configure();
		
		// Run() reloads through PluginProject, Configure() doesn't
		LoadProject();
		
		Workbench.ScriptDialog("Edit Environment", DIALOG_TAB_SIZE, m_LaunchSettings);
	}
}