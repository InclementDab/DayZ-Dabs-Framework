class TypeConversionString: TypeConversionTemplate<string>
{
	override bool GetBool()
	{
		return string.ToString(m_Value, false, false, false) == "1";
	}

	override void SetBool(bool value)
	{
		m_Value = string.ToString(value, false, false, false);
	}

	override int GetInt()
	{
		return m_Value.ToInt();
	}

	override void SetInt(int value)
	{
		m_Value = string.ToString(value, false, false, false);
	}

	override float GetFloat()
	{
		return m_Value.ToFloat();
	}

	override void SetFloat(float value)
	{
		m_Value = string.ToString(value, false, false, false);
	}

	override vector GetVector()
	{
		return m_Value.ToVector();
	}

	override void SetVector(vector value)
	{
		m_Value = value.ToString(false);
	}

	override string GetString()
	{
		return m_Value;
	}

	override void SetString(string value)
	{
		m_Value = value;
	}
}