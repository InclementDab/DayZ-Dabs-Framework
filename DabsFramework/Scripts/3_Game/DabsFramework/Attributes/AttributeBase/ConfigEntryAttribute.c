class ConfigEntryAttribute: AttributeBase
{	
	protected string m_Name;
	protected typename m_Force;
	protected ref ScriptCaller m_OnChanged;
	
	void ConfigEntryAttribute(string variable_name = string.Empty, typename force_type = EMPTY_TYPENAME, ScriptCaller on_changed = null)
	{
		m_Name = variable_name;
		m_Force = force_type;
		m_OnChanged = on_changed;
	}
	
	string GetVariableName()
	{
		return Ternary<string>.If(m_Name != string.Empty, m_Name, Field.Name);
	}
	
	typename GetVariableType()
	{
		return Ternary<typename>.If(m_Force != EMPTY_TYPENAME, m_Force, Field.DeclaringType);
	}
	
	ScriptCaller GetOnChanged()
	{
		return m_OnChanged;
	}
}