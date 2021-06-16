class TypeConversionWidget: TypeConversionTemplate<Widget>
{
	override void SetString(string value)
	{
		m_Value = GetWorkbenchGame().GetWorkspace().CreateWidgets(value);
	}

	override void SetWidget(Widget value)
	{
		m_Value = value;
	}

	override Widget GetWidget()
	{
		return m_Value;
	}
}