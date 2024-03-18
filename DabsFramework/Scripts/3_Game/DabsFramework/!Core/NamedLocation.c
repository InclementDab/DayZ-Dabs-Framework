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
	static const string BIGMILITARY = "BigMilitary";
	static const string MEDMILITARY = "MediumMilitary";
	static const string SMALLMILITARY = "SmallMilitary";
	static const string FEEDSHACK = "FeedShack";
	static const string DEERSTAND = "DeerStand";
	static const string TRADER = "Trader";
	
	[ConfigEntryAttribute("name")]
	string Name;
	
	[ConfigEntryAttribute("position", vector, ScriptCaller.Create(OnPositionUpdate))]
	vector Position;
	
	[ConfigEntryAttribute("type")]
	string Type;
	
	Symbols GetIcon()
	{
		switch (Type) {
			case CAPITAL: return Symbols.CITY;
			case TRADER: return Symbols.COIN;
			/*case CITY:
			case VILLAGE: return string.Empty;
			case CAMP: return Symbols.CAMPFIRE;
			case LOCAL: return string.Empty;
			case HILL: return Symbols.MOUNTAIN;
			case MARINE: return Symbols.WAVE;
			case RUIN: return Symbols.HILL_ROCKSLIDE;
			case RAILROAD: return Symbols.TRAIN;
			case OFFICE: return Symbols.BUILDING;
			case VIEWPOINT: return Symbols.BINOCULARS;
			case BIGMILITARY: return Symbols.PERSON_RIFLE;
			case MEDMILITARY: return Symbols.SHIELD;
			case SMALLMILITARY: return Symbols.KNIFE;
			case FEEDSHACK: return Symbols.WHEAT;
			case DEERSTAND: return Symbols.TOWER_OBSERVATION;*/
		}
		
		return string.Empty;
	}
	
	float GetImportance()
	{
		switch (Type) {
			case CAPITAL:
			case CITY: return 2.0;
			case VILLAGE:
			case CAMP: return 0.65;
			case LOCAL:
			case HILL:
			case MARINE:
			case RUIN:
			case RAILROAD:
			case OFFICE:
			case VIEWPOINT: return 0.22;
			case BIGMILITARY: return 0.65;
			case MEDMILITARY: return 0.5;
			case SMALLMILITARY: return 0.35;
			case FEEDSHACK: return 0.2;
			case DEERSTAND: return 0.1;
			case TRADER: return 2.0;
		}
		
		return 1.0;
	}
	 	
	LinearColor GetColor()
	{
		switch (Type) {
			case TRADER: return LinearColor.GOLD;
			case CAPITAL:
			case CITY:
			case VILLAGE:
			case CAMP: return LinearColor.BLACK;
			case LOCAL:
			case HILL: return LinearColor.BROWN;
			case MARINE:
			case RUIN:
			case RAILROAD:
			case OFFICE: 
			case VIEWPOINT:
			case BIGMILITARY: 
			case MEDMILITARY: 
			case SMALLMILITARY: 
			case FEEDSHACK:
			case DEERSTAND: return LinearColor.BLACK; 
		}
		
		return LinearColor.BLACK;
	}
	
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