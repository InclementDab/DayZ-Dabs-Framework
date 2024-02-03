class ProfileSettingColor: ProfileSettingEntry
{
	protected bool m_AllowAlpha;
	
	void ProfileSettingColor(string variable_name, string display_name, bool require_restart, bool allow_alpha)
	{
		m_AllowAlpha = allow_alpha;
	}
	
	override ScriptView CreateMenuOption(notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		return new OptionSelectorColorView(settings, on_changed, this);
	}
	
	bool GetAllowAlpha()
	{
		return m_AllowAlpha;
	}
}