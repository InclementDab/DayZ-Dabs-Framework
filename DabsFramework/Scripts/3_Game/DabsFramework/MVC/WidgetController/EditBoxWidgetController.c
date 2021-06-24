class EditBoxWidgetController : WidgetControllerTemplate<EditBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetString(m_Widget.GetText());
	}
}