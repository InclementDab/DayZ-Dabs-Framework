[WorkbenchPluginAttribute("Configure Project", "Configure DayZ Project", "Ctrl+F6", "", {"ScriptEditor", "ResourceManager"})]
class PluginConfigureProject: PluginProject
{
	override void Run()
	{
		Workbench.ScriptDialog("Edit Environment", DIALOG_TAB_SIZE, m_WorkbenchSettings);
	}
	
	override void Configure()
	{
		Workbench.ScriptDialog("Edit Environment", DIALOG_TAB_SIZE, m_WorkbenchSettings);
	}
}