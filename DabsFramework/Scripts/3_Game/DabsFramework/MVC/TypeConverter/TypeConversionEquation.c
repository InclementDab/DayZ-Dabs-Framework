class TypeConversionEquation: TypeConversionTemplate<StringEvaluater>
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
		return m_Value.Parse();
	}

	override void SetInt(int value)
	{
		m_Value = value.ToString();
	}

	override float GetFloat()
	{
		return m_Value.Parse();
	}

	override void SetFloat(float value)
	{
		m_Value = value.ToString();
	}
	
	override void SetString(string value) 
	{
		m_Value = string.Empty;
		for (int i = 0; i < value.Length(); i++) {
			int ascii = value[i].Hash();
			if (ascii >= 40 && ascii <= 57 || ascii == 32 || ascii == 94) {
				m_Value += value[i];
			}
		}
	}
	
	override string GetString() 
	{
		return m_Value;
	}
}