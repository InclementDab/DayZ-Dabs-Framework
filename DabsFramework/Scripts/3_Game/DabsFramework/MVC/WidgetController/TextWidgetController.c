class TextWidgetController : WidgetControllerTemplate<TextWidget>
{
	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}
}