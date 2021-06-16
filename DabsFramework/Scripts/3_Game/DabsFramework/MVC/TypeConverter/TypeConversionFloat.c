class TypeConversionFloat: TypeConversionTemplate<float>
{
	override bool GetBool()
	{
		return m_Value;
	}

	override void SetBool(bool value)
	{
		m_Value = value;
	}

	override int GetInt()
	{
		return m_Value;
	}

	override void SetInt(int value)
	{
		m_Value = value;
	}

	override float GetFloat()
	{
		return m_Value;
	}

	override void SetFloat(float value)
	{
		m_Value = value;
	}

	override string GetString()
	{
		return string.ToString(m_Value, false, false, false);
	}

	override void SetString(string value)
	{
		m_Value = value.ToFloat();
	}
}