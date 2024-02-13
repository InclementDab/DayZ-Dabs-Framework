// * see: ConfigEntryAttribute
class ConfigObject: Managed
{
	protected string m_Path;
	protected ref array<string> m_FullPath = {};
	protected string m_Name;
	
	void ConfigObject(string path)
	{
		m_Path = path;
		m_Path.Split(" ", m_FullPath);
		m_Name = m_FullPath[Math.Max(0, m_FullPath.Count() - 1)];
		
		array<AttributeBase> attributes = AttributeBase.GetCustomAttributes(Type());
		foreach (AttributeBase attribute: attributes) {
			ConfigEntryAttribute config_entry_attribute = ConfigEntryAttribute.Cast(attribute);
			if (!config_entry_attribute || !config_entry_attribute.Field || !GetGame()) {
				continue;
			}
			
			typename variable_type = config_entry_attribute.GetVariableType();
			string config_entry = string.Format("%1 %2", path, config_entry_attribute.GetVariableName());			
			switch (variable_type) {
				case bool:
				case int: {
					EnScriptVar<int>.Set(this, config_entry_attribute.Field.Name, GetGame().ConfigGetInt(config_entry));
					break;
				}
				
				case float: {
					EnScriptVar<float>.Set(this, config_entry_attribute.Field.Name, GetGame().ConfigGetFloat(config_entry));
					break;
				}
				
				case vector: {
					EnScriptVar<vector>.Set(this, config_entry_attribute.Field.Name, GetGame().ConfigGetVector(config_entry));
					break;
				}
				
				case string: {
					EnScriptVar<string>.Set(this, config_entry_attribute.Field.Name, GetGame().ConfigGetTextOut(config_entry));
					break;
				}
				
				case array: {
					Class array_reference = config_entry_attribute.Field.DeclaringType.Spawn();
					EnScript.SetClassVar(this, config_entry_attribute.Field.Name, 0, array_reference);
					
					array<string> raw_entries = {};					
					GetGame().ConfigGetTextArrayRaw(config_entry, raw_entries);
					foreach (string raw: raw_entries) {
						vector vector_tryparse = raw.ToVector();
						if (vector_tryparse != vector.Zero) {
							//g_Script.Call(array_reference, "Insert", vector_tryparse);
						}
					}
					
					break;
				}
				
				default: {
					PrintFormat("Unhandled variable type:%1 name:%2", variable_type, config_entry_attribute.Field.Name);
					break;
				}
			}
			
			if (config_entry_attribute.GetOnChanged()) {
				config_entry_attribute.GetOnChanged().Invoke(this);
			}
		}
	}
	
	string GetPath()
	{
		return m_Path;
	}
	
	array<string> GetFullPath()
	{
		return m_FullPath;
	}
	
	string GetName()
	{
		return m_Name;
	}
}