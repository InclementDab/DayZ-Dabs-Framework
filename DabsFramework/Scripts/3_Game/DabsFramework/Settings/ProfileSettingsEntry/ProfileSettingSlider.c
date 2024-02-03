class ProfileSettingSlider: ProfileSettingEntry
{
	protected float m_MinValue, m_MaxValue;
	
	void ProfileSettingSlider(string variable_name, string display_name, bool require_restart, float min, float max)
	{
		m_MinValue = min;
		m_MaxValue = max;
	}
	
	override ScriptView CreateMenuOption(notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		return new OptionSelectorSliderView(settings, on_changed, this);
	}
	
	float GetMin()
	{
		return m_MinValue;
	}
	
	float GetMax()
	{
		return m_MaxValue;
	}
}