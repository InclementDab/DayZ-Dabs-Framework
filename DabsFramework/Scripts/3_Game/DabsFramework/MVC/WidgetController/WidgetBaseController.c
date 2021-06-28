class WidgetBaseController : WidgetControllerTemplate<Widget>
{
	override void Set(TypeConverter type_converter)
	{
		if (!m_Widget) {
			return;
		}

		Widget parent = m_Widget.GetParent();
		int sort = m_Widget.GetSort();
		if (type_converter.GetWidget()) {
			m_Widget = type_converter.GetWidget();

			if (parent) {
				parent.AddChild(m_Widget, false);
			}

			m_Widget.SetSort(sort);
		}
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.SetWidget(m_Widget);
	}
	
	override void SetSelection(TypeConverter type_converter)
	{
		if (m_Widget) {
			m_Widget.SetColor(type_converter.GetInt());
		}
	}
}