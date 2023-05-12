class ProfileSettings: Class
{
	protected ref PropertyTypeHashMap m_PropertyTypeHashMap = new PropertyTypeHashMap(Type());
	
	private void ProfileSettings()
	{
	}
	
	private void ~ProfileSettings()
	{
		delete m_PropertyTypeHashMap;
	}
	
	bool Save()
	{		
		if (!GetGame()) {
			return false;
		}
		
		foreach (string variable_name, typename variable_type: m_PropertyTypeHashMap) {
			switch (variable_type) {
				case bool: {
					bool _bool;
					EnScript.GetClassVar(this, variable_name, 0, _bool);
					GetGame().SetProfileString(FormatSaveName(variable_name), string.ToString(_bool));
					GetGame().SaveProfile();
					break;
				}
				
				case int: {
					int _int;
					EnScript.GetClassVar(this, variable_name, 0, _int);
					GetGame().SetProfileString(FormatSaveName(variable_name), string.ToString(_int));
					GetGame().SaveProfile();
					break;
				}
				
				case float: {
					float _float;
					EnScript.GetClassVar(this, variable_name, 0, _float);
					GetGame().SetProfileString(FormatSaveName(variable_name), string.ToString(_float));
					GetGame().SaveProfile();
					break;
				}
				
				case string: {
					string _string;
					EnScript.GetClassVar(this, variable_name, 0, _string);
					GetGame().SetProfileString(FormatSaveName(variable_name), _string);
					GetGame().SaveProfile();
					break;
				}
				
				case array: {
					//array<string> string_casted_values = {};
					
					//GetGame().SetProfileStringList();
					break;
				}
				
				default: {
					Error(string.Format("ProfileSettings::Save Unexpected variable type=%1 name=%2", variable_type, variable_name));
					return false;
				}
			}
		}
		
		return true;
	}
	
	bool Load()
	{
		if (!GetGame()) {
			return false;
		}
		
		foreach (string variable_name, typename variable_type: m_PropertyTypeHashMap) {			
			switch (variable_type) {				
				case bool: {
					bool _bool;
					EnScript.GetClassVar(this, variable_name, 0, _bool);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileBool(FormatSaveName(variable_name), _bool));
					break;
				}
				
				case int: {					
					int _int;
					EnScript.GetClassVar(this, variable_name, 0, _int);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileInt(FormatSaveName(variable_name), _int));
					break;
				}
				
				case float: {
					float _float;
					EnScript.GetClassVar(this, variable_name, 0, _float);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileFloat(FormatSaveName(variable_name), _float));
					break;
				}
				
				case string: {
					string _string;
					EnScript.GetClassVar(this, variable_name, 0, _string);
					EnScript.SetClassVar(this, variable_name, 0, GetProfileString(FormatSaveName(variable_name), _string));
					break;
				}
				
				default: {
					Error(string.Format("ProfileSettings::Load Unexpected variable type=%1 name=%2", variable_type, variable_name));
					break;
				}
			}
		}
		
		return false;
	}
		
	static bool GetProfileBool(string variable, bool default = false)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return (value == "true" || value == "1");
		}
		
		return default;
	}
	
	static float GetProfileFloat(string variable, float default = 0)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value.ToFloat();
		}
		
		return default;
	}
	
	static int GetProfileInt(string variable, int default = 0)
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value.ToInt();
		}
		
		return default;
	}
	
	static string GetProfileString(string variable, string default = "")
	{
		string value;
		if (GetGame().GetProfileString(variable, value)) {
			return value;
		}
		
		return default;
	}
	
	string FormatSaveName(string variable_name)
	{
		return string.Format("%1.%2", Type(), variable_name);
	}
}