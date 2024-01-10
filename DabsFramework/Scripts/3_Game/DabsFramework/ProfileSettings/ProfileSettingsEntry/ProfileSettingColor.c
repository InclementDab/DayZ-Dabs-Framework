class ProfileSettingColor: ProfileSettingEntry
{
	override ScriptView CreateMenuOption(notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		return new OptionSelectorColorView(settings, on_changed, this);
	}
}