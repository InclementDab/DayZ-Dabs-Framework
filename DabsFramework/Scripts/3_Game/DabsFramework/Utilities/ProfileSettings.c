class ProfileSettings: Class
{	
	// Ctor immediately loads all settings
	private void ProfileSettings()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(Type());
		foreach (string variable_name, typename variable_type: properties) {
			string variable_name_formatted = GetFormattedSaveName(variable_name);
			
			string variable_value_string;
			if (!g_Game.GetProfileString(variable_name_formatted, variable_value_string)) {
				continue;
			}
						
			switch (variable_type) {
				case bool: {
					EnScript.SetClassVar(this, variable_name, 0, GetProfileBool(variable_name_formatted, EnScriptVar<bool>.Get(this, variable_name)));
					break;
				}
				
				case int: {
					EnScript.SetClassVar(this, variable_name, 0, GetProfileInt(variable_name_formatted, EnScriptVar<int>.Get(this, variable_name)));
					break;
				}
				
				case float: {
					EnScript.SetClassVar(this, variable_name, 0, GetProfileFloat(variable_name_formatted, EnScriptVar<float>.Get(this, variable_name)));
					break;
				}
				
				case string: {
					EnScript.SetClassVar(this, variable_name, 0, GetProfileString(variable_name_formatted, EnScriptVar<string>.Get(this, variable_name)));
					break;
				}
				
				case String("array<string>").ToType(): {
					EnScript.SetClassVar(this, variable_name, 0, GetProfileStringList(variable_name_formatted, EnScriptVar<array<string>>.Get(this, variable_name)));
				}
				
				default: {
					Error(string.Format("ProfileSettings::Load Unsupported variable type=%1 name=%2", variable_type, variable_name));
					break;
				}
			}
		}
	}
	
	private void ~ProfileSettings()
	{
	}
	
	void Save()
	{
		if (!g_Game) {
			return;
		}
				
		// iterate though all properties of the class
		PropertyTypeHashMap properties = new PropertyTypeHashMap(Type());
		foreach (string variable_name, typename variable_type: properties) {		
			string variable_name_formatted = GetFormattedSaveName(variable_name);			
				
			switch (variable_type) {
				case bool: {
					g_Game.SetProfileString(variable_name_formatted, string.ToString(EnScriptVar<bool>.Get(this, variable_name)));
					break;
				}
				
				case int: {
					g_Game.SetProfileString(variable_name_formatted, string.ToString(EnScriptVar<int>.Get(this, variable_name)));
					break;
				}
				
				case float: {
					g_Game.SetProfileString(variable_name_formatted, string.ToString(EnScriptVar<float>.Get(this, variable_name)));
					break;
				}
				
				case string: {
					g_Game.SetProfileString(variable_name_formatted, string.ToString(EnScriptVar<string>.Get(this, variable_name)));
					break;
				}
				
				case String("array<string>").ToType(): {
					g_Game.SetProfileStringList(variable_name_formatted, EnScriptVar<array<string>>.Get(this, variable_name));
					break;
				}
												
				default: {
					Error(string.Format("ProfileSettings::Save Unsupported variable type=%1 name=%2", variable_type, variable_name));					
					break;
				}
			}
		}
		
		g_Game.SaveProfile();
	}
			
	static bool GetProfileBool(string variable, bool default = false)
	{
		string value;
		if (g_Game.GetProfileString(variable, value)) {
			return (value == "true" || value == "1");
		}
		
		return default;
	}
	
	static float GetProfileFloat(string variable, float default = 0)
	{
		string value;
		if (g_Game.GetProfileString(variable, value)) {
			return value.ToFloat();
		}
		
		return default;
	}
	
	static int GetProfileInt(string variable, int default = 0)
	{
		string value;
		if (g_Game.GetProfileString(variable, value)) {
			return value.ToInt();
		}
		
		return default;
	}
	
	static string GetProfileString(string variable, string default = "")
	{
		string value;
		if (g_Game.GetProfileString(variable, value)) {
			return value;
		}
		
		return default;
	}
	
	static array<string> GetProfileStringList(string variable, array<string> default)
	{
		array<string> value = {};
		g_Game.GetProfileStringList(variable, value);
		if (!value) {
			return default;
		}
		
		return value;
	}
	
	string GetFormattedSaveName(string variable_name)
	{
		return string.Format("%1.%2", Type(), variable_name);
	}
}