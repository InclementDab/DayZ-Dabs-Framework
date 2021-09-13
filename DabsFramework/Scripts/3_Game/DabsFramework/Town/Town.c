class Town
{
	string Entry;
	string Name;
	TownFlags Type;
	vector Position;
	
	// default world_name parameter will load the active world
	// 3 is TownFlags.CAPITAL | TownFlags.CITY, but Enfusion is big silly
	static array<ref Town> GetMapTowns(TownFlags flags = 3, string world_name = "")
	{
		if (world_name == string.Empty) {
			GetGame().GetWorldName(world_name);
		}
		
		array<ref Town> town_positions = {};
		string cfg = "CfgWorlds " + world_name + " Names";
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string city;
			GetGame().ConfigGetChildName(cfg, i, city);	
			vector city_position; 
			TFloatArray float_array = {};
			GetGame().ConfigGetFloatArray(string.Format("%1 %2 position", cfg, city), float_array);
			city_position[0] = float_array[0]; city_position[2] = float_array[1];
			city_position[1] = GetGame().SurfaceY(city_position[0], city_position[2]);
			
			TownFlags flag = GetTownFlag(GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city)));
			if ((flags & flag) != flag) {
				continue;
			}
			
			Town town_entry();
			town_entry.Entry = city;
			town_entry.Type = flag;
			town_entry.Name = GetGame().ConfigGetTextOut(string.Format("%1 %2 name", cfg, city));
			town_entry.Position = city_position;
			town_positions.Insert(town_entry);
		}
		
		return town_positions;
	}
	
	static string GetTownTypeString(TownFlags flag)
	{
		switch (flag) {
			case TownFlags.CAPITAL: return "Capital";
			case TownFlags.CITY: 	return "City";
			case TownFlags.VILLAGE: return "Village";
			case TownFlags.CAMP: 	return "Camp";
			case TownFlags.LOCAL: 	return "Local";
			case TownFlags.HILL: 	return "Hill";
			case TownFlags.MARINE: 	return "Marine";
			case TownFlags.RUIN: 	return "Ruin";
			case TownFlags.RAILROAD:return "RailroadStation";
			case TownFlags.OFFICE: 	return "LocalOffice";
			case TownFlags.VIEWPOINT:return "ViewPoint";
		}
		
		return string.Empty;
	}
	
	static int GetTownFlag(string town_type)
	{
		switch (town_type) {
			case "Capital": 	return TownFlags.CAPITAL;
			case "City": 		return TownFlags.CITY;
			case "Village": 	return TownFlags.VILLAGE;
			case "Camp": 		return TownFlags.CAMP;
			case "Local": 		return TownFlags.LOCAL;
			case "Hill": 		return TownFlags.HILL;
			case "Marine": 		return TownFlags.MARINE;
			case "Ruin": 		return TownFlags.RUIN;
			case "RailroadStation": return TownFlags.RAILROAD;
			case "LocalOffice": return TownFlags.OFFICE;
			case "ViewPoint": 	return TownFlags.VIEWPOINT;
		}
		
		return 0;
	}
	
	// Should be expanded or modded
	int GetSize()
	{
		switch (Type) {
			case TownFlags.CAPITAL: return 500;
			case TownFlags.CITY: 	return 300;
			case TownFlags.VILLAGE: return 100;
		}
		
		return 0;
	}
	
	float GetDistance()
	{	
		return vector.Distance(GetGame().GetPlayer().GetPosition(), Position);
	}
}