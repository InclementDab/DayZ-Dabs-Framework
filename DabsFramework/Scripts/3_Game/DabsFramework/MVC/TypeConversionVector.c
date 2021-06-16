class TypeConversionVector: TypeConversionTemplate<vector>
{
	override vector GetVector()
	{
		return m_Value;
	}

	override void SetVector(vector value)
	{
		m_Value = value;
	}

	override string GetString()
	{
		return m_Value.ToString(false);
	}

	override void SetString(string value)
	{
		m_Value = value.ToVector();
	}
}