class NamedLocation: ConfigObject
{
	static const string CAPITAL = "Capital";
	static const string CITY = "City";
	static const string VILLAGE = "Village";
	static const string CAMP = "Camp";
	static const string LOCAL = "Local";
	static const string HILL = "Hill";
	static const string MARINE = "Marine";
	static const string RUIN = "Ruin";
	static const string RAILROAD = "RailroadStation";
	static const string OFFICE = "LocalOffice";
	static const string VIEWPOINT = "ViewPoint";
	
	[ConfigEntryAttribute("name")]
	string Name;
	
	[ConfigEntryAttribute("position", vector, ScriptCaller.Create(OnPositionUpdate))]
	vector Position;
	
	[ConfigEntryAttribute("type")]
	string Type;
	
	static void OnPositionUpdate(notnull NamedLocation instance)
	{
		instance.Position = Vector(instance.Position[0], GetGame().SurfaceY(instance.Position[0], instance.Position[1]), instance.Position[1]);
	}
	
	static array<ref NamedLocation> EnumerateAllLocations()
	{
		array<ref NamedLocation> towns = {};
		string cfg = string.Format("CfgWorlds %1 Names", GetDayZGame().GetWorldNameEx(false));
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string name;
			if (!GetGame().ConfigGetChildName(cfg, i, name)) {
				continue;
			}
			
			towns.Insert(new NamedLocation(string.Format("%1 %2", cfg, name)));
		}
		
		return towns;
	}
	
#ifdef DIAG_DEVELOPER
	void Debug()
	{
		PrintFormat("Config:%4 Name:%1, Position:%2, Type:%3", Name, Position.ToString(), Type, GetName());
	}
#endif
}