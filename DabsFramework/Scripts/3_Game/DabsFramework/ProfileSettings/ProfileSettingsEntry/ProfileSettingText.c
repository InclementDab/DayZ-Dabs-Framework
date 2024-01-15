class ProfileSettingText: ProfileSettingEntry
{
	override ScriptView CreateMenuOption(notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		return new OptionSelectorEditboxView(settings, on_changed, this);
	}
}