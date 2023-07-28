class TextWidgetController : WidgetControllerTemplate<TextWidget>
{
	override void Set(TypeConverter type_converter)
	{
		if (!m_Widget) {
			return;
		}
		
		m_Widget.SetText(type_converter.GetString());
	}

	override void Get(out TypeConverter type_converter)
	{
		string text;
		m_Widget.GetText(text);
		type_converter.SetString(text);
	}
}