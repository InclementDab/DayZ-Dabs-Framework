class ProfileSettingMultistate: ProfileSettingEntry
{
	protected ref array<string> m_PossibleStates = {};
	
	void ProfileSettingMultistate(string variable_name, string display_name, bool require_restart, notnull array<string> possible_states)
	{
		m_PossibleStates.Copy(possible_states);
	}
	
	override OptionSelectorViewBase CreateMenuOption(notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		return new OptionSelectorMultistateView(settings, on_changed, this);
	}
	
	array<string> GetPossibleStates()
	{
		return m_PossibleStates;
	}
}