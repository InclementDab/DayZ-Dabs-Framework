class ProfileSettingEntry: Managed
{
	protected string m_VariableName;
	protected string m_DisplayName;
	protected bool m_RequiresRestart;
	
	void ProfileSettingEntry(string variable_name, string display_name, bool require_restart)
	{
		m_VariableName = variable_name;
		m_DisplayName = display_name;
		m_RequiresRestart = require_restart;
	}
	
	string GetVariableName()
	{
		return m_VariableName;
	}
	
	string GetDisplayName()
	{
		return m_DisplayName;
	}
	
	bool GetRequiresRestart()
	{
		return m_RequiresRestart;
	}
	
	OptionSelectorViewBase CreateMenuOption(notnull ProfileSettings settings, notnull ScriptCaller on_changed);
}