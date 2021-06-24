class SliderWidgetController : WidgetControllerTemplate<SliderWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetCurrent(type_converter.GetFloat());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetFloat(m_Widget.GetCurrent());
	}
}