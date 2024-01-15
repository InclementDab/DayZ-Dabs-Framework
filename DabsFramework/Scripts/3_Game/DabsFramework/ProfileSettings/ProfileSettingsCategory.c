class ProfileSettingsCategory: Managed
{
	string Header;
	ref array<ref ProfileSettingEntry> Settings = {};
	
	void ProfileSettingsCategory(string header)
	{
		Header = header;
	}
	
	void ~ProfileSettingsCategory()
	{
		delete Settings;
	}
	
	void Set(int index, ProfileSettingEntry settings_entry)
	{
		Settings[index] = settings_entry;
	}
	
	ProfileSettingEntry Get(int index)
	{
		return Settings[index];
	}
}