class TypeConversionObservable: TypeConversionTemplate<Observable>
{
	override int GetInt()
	{
		return m_Value.Count();
	}

	override string GetString()
	{
		return m_Value.Count().ToString();
	}
}