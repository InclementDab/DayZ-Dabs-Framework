class RegisterProfileSettingText: RegisterProfileSettingBase
{
	void RegisterProfileSettingText(string category_name, string variable_name, string display_name, bool require_restart = false)
	{
		typename profile_settings_type = GetCurrentProfileSetting();
		
		// find existing category
		array<ref ProfileSettingsCategory> categories = Instances[profile_settings_type];
		foreach (ProfileSettingsCategory category: categories) {
			if (category && category.Header == category_name) {
				category.Settings.Insert(new ProfileSettingText(variable_name, display_name, require_restart));
				return; // we found it
			}
		}
			
		// create new category
		ProfileSettingsCategory new_category = new ProfileSettingsCategory(category_name);
		new_category.Settings.Insert(new ProfileSettingText(variable_name, display_name, require_restart));
		Instances[profile_settings_type].Insert(new_category);
	}
}