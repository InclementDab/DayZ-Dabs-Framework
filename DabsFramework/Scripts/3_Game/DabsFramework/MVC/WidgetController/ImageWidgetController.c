class ImageWidgetController : WidgetControllerTemplate<ImageWidget>
{
	override void Set(TypeConverter type_converter)
	{
		if (type_converter.GetString())
		{
			m_Widget.LoadImageFile(0, type_converter.GetString());
			m_Widget.SetImage(0);
		}
	}
}