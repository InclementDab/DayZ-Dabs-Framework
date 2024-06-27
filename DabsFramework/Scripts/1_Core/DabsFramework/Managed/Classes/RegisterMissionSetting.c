class RegisterMissionSetting: Class
{
	static ref map<typename, string> s_RegisteredInstances = new map<typename, string>();

	void RegisterMissionSetting(typename mission_setting_type, string file_path)
	{
		if (!Path.IsPathRooted(file_path)) {
			file_path = Path.Combine(Path.Mission(), file_path);
		}

		if (mission_setting_type.IsInherited("MissionSetting".ToType())) {
			s_RegisteredInstances[mission_setting_type] = file_path;
		} ErrorEx("RegisterMissionSetting must be used with type MissionSetting");
	}
}