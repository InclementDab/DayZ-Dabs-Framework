class TypeConversionScriptView: TypeConversionTemplate<ScriptedViewBase>
{
	override Widget GetWidget()
	{
		// Todo: why can this be null? not sure
		if (m_Value) {
			return m_Value.GetLayoutRoot();
		}

		return null;
	}

	override void SetWidget(Widget value)
	{
		m_Value.OnWidgetScriptInit(value);
	}
}