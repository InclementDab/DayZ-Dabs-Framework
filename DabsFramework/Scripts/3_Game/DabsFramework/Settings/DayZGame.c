
modded class DayZGame
{
	protected ref map<typename, ref MissionSetting> m_MissionSettings = new map<typename, ref MissionSetting>();

	override void SetMissionPath(string path)
	{
		super.SetMissionPath(path);

#ifdef SERVER
		foreach (typename mission_setting_type, string mission_setting_file: RegisterMissionSetting.s_RegisteredInstances) {
			bool file_exists = File.Exists(mission_setting_file);
			JsonSerializer json_file_serializer = new JsonSerializer();
			MissionSetting mission_setting = MissionSetting.Cast(mission_setting_type.Spawn());
			if (!mission_setting) {
				ErrorEx(string.Format("failed to create mission setting type: %1, file: %2", mission_setting_type, mission_setting_file));
				break;
			}
			
			if (!file_exists) {
				bool save_success = mission_setting.Save();
                if (!save_success) {
                    ErrorEx(string.Format("failed to save mission settings"));
                    break;
                }
			} else {
				if (!json_file_serializer.CanRead()) {
					ErrorEx(string.Format("failed read from json file serializer setting type: %1, file: %2", mission_setting_type, mission_setting_file));
					break;
				}

				string file_text = File.ReadAllText(mission_setting_file);
				if (!file_text) {
					ErrorEx(string.Format("empty json file found"));
					break;
				}

				string json_error;
				if (!json_file_serializer.ReadFromString(mission_setting, file_text, json_error)) {
					PrintFormat("json error, file: %1, error: %2", mission_setting_file, json_error);
					break;
				}
			}

			m_MissionSettings[mission_setting_type] = mission_setting;
		}
#endif
	}

	MissionSetting GetMissionSetting(typename mission_settings)
	{
		return m_MissionSettings[mission_settings];
	}
}