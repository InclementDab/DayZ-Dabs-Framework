class TypeConversionObject: TypeConversionTemplate<Object>
{
	override float GetFloat()
	{
		return m_Value.GetID();
	}
	
	override int GetInt()
	{
		return m_Value.GetID();
	}
	
	override string GetString()
	{
		return m_Value.GetType();
	}

	override void SetString(string value)
	{
		m_Value = GetGame().CreateObjectEx(value, vector.Zero, ECE_NONE);
	}

	override void SetObject(Object value)
	{
		m_Value = value;
	}

	override Object GetObject()
	{
		return m_Value;
	}
}