class TextWidgetController : WidgetControllerTemplate<TextWidget>
{
	override void Set(TypeConverter type_converter)
	{
		if (!m_Widget) {
			return;
		}
		
		m_Widget.SetText(type_converter.GetString());
	}
}