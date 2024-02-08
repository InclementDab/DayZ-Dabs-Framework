class RegisterProfileSettingBase: Class
{
	static ref map<typename, ref array<ref ProfileSettingsCategory>> Instances;
	
	void RegisterProfileSettingBase(string category_name, string variable_name, string display_name)
	{
		// Get last registered instance
		if (!RegisterProfileSetting.Instances) {
			Error("RegisterProfileSettings must be called for RegisterProfileSettingSwitch to function");
			return;
		}
		
		if (!Instances) {
			Instances = new map<typename, ref array<ref ProfileSettingsCategory>>();
		}
	}
	
	// sequential compilation programming. what the hell
	typename GetCurrentProfileSetting()
	{
		typename profile_settings_type = RegisterProfileSetting.Instances[RegisterProfileSetting.Instances.Count() - 1].param1;
		if (!Instances[profile_settings_type]) {
			Instances.Insert(profile_settings_type, {});
		}
		
		return profile_settings_type;
	}
}