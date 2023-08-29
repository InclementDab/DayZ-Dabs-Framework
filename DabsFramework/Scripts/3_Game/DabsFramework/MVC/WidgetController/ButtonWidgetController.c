class ButtonWidgetController : WidgetControllerTemplate<ButtonWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetState(type_converter.GetBool());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetBool(m_Widget.GetState());
	}

	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}
	
#ifdef DAYZ_1_22
	override void GetSelection(out TypeConverter type_converter)
	{
		string text;
		m_Widget.GetText(text);
		type_converter.SetString(text);
	}
#endif
}