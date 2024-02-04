class TownType: string
{
	static const TownType CAPITAL = "Capital";
	static const TownType CITY = "City";
	static const TownType VILLAGE = "Village";
	static const TownType CAMP = "Camp";
	static const TownType LOCAL = "Local";
	static const TownType HILL = "Hill";
	static const TownType MARINE = "Marine";
	static const TownType RUIN = "Ruin";
	static const TownType RAILROAD = "RailroadStation";
	static const TownType OFFICE = "LocalOffice";
	static const TownType VIEWPOINT = "ViewPoint";
}

typedef string TownType;

class Town: ConfigObject
{
	[ConfigEntryAttribute("name")]
	string Name;
	
	[ConfigEntryAttribute("position", vector, ScriptCaller.Create(OnPositionUpdate))]
	vector Position;
	
	[ConfigEntryAttribute("type")]
	TownType Type;
	
	static void OnPositionUpdate(notnull Town instance)
	{
		instance.Position = Vector(instance.Position[0], GetGame().SurfaceY(instance.Position[0], instance.Position[1]), instance.Position[1]);
	}
	
	static array<ref Town> EnumerateAllTowns()
	{
		array<ref Town> towns = {};
		string cfg = string.Format("CfgWorlds %1 Names", GetDayZGame().GetWorldNameEx(false));
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string name;
			if (!GetGame().ConfigGetChildName(cfg, i, name)) {
				continue;
			}
			
			towns.Insert(new Town(string.Format("%1 %2", cfg, name)));
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