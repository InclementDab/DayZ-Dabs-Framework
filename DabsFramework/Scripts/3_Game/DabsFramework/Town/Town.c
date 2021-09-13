class Town
{
	string Name;
	string Type;
	vector Position;
	
	int GetSize()
	{
		switch (Type) {
			case "Capital": return 500;
			case "City": 	return 300;
			case "Village": return 100;
			
			// This is for incorrectly configured maps
			case "NameCapital": return 500;
			case "NameCity": return 300;
			case "NameVillage": return 100;
		}
		
		return 0;
	}
	
	float GetDistance()
	{	
		return vector.Distance(GetGame().GetPlayer().GetPosition(), Position);
	}
	
	// default world_name parameter will load the active world
	static array<ref Town> GetMapTowns(string world_name = "")
	{
		if (world_name == string.Empty) {
			GetGame().GetWorldName(world_name);
		}
		
		array<ref Town> town_positions = {};
		string cfg = "CfgWorlds " + world_name + " Names";		
		string allowed_types = "Capital City Village NameCapital NameCity NameVillage";
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string city;
			GetGame().ConfigGetChildName(cfg, i, city);	
			vector city_position; 
			TFloatArray float_array = {};
			GetGame().ConfigGetFloatArray(string.Format("%1 %2 position", cfg, city), float_array);
			city_position[0] = float_array[0]; city_position[2] = float_array[1];
			city_position[1] = GetGame().SurfaceY(city_position[0], city_position[2]);
				
			if (allowed_types.Contains(GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city)))) {
				Town town_entry();
				town_entry.Type = GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city));
				town_entry.Name = GetGame().ConfigGetTextOut(string.Format("%1 %2 name", cfg, city));
				town_entry.Position = city_position;
				town_positions.Insert(town_entry);
			}
		}
		
		return town_positions;
	}
}