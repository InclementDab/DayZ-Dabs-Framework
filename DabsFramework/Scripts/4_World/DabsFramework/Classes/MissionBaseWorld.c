modded class MissionBaseWorld
{
	MissionSettingBase GetSetting(typename mission_setting_type)
	{
		return GetDayZGame().GetMissionSetting(mission_setting_type);
	}
}