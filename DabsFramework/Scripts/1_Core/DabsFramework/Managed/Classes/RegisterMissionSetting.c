class RegisterMissionSetting: Class
{
	static ref map<typename, string> s_RegisteredInstances = new map<typename, string>();

	void RegisterMissionSetting(typename mission_type, string file_path)
	{		
		s_RegisteredInstances[mission_type] = file_path;
	}
}