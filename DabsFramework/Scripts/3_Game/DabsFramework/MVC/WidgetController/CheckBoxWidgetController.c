class CheckBoxWidgetController : WidgetControllerTemplate<CheckBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetChecked(type_converter.GetBool());
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetBool(m_Widget.IsChecked());
	}

	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		// Since SetSelection is being used. This needs to override to avoid errors
	}
}