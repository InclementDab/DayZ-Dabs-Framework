class TypeConversionObject: TypeConversionTemplate<Object>
{
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