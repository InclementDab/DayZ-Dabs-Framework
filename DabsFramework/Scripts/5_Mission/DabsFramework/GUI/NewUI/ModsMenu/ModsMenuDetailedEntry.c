modded class ModsMenuDetailedEntry
{
	// Attempting to fix the lack of overview with this
	override void LoadData()
	{
		super.LoadData();
		
		string description = m_Data.GetOverview();		
		
		if (description == "") {
			description = TryGetDescription(m_Data);
		}
		
		if (description != "") {
			m_Description.SetText(description);
			m_Description.Update();
			m_Detail.Update();
		}
	}
	
	static string TryGetDescription(ModInfo mod_info)
	{
		string base_dir = "CfgMods";
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(base_dir); i++) {
			string child;
			GetGame().ConfigGetChildName(base_dir, i, child);
			string name = GetGame().ConfigGetTextOut(base_dir + " " + child + " name");
			
			if (mod_info.GetName() == name) {
				return GetGame().ConfigGetTextOut(base_dir + " " + child + " overview");
			}
		}
		
		return string.Empty;
	}
}
	