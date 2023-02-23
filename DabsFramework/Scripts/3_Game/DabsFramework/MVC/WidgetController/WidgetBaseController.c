class WidgetBaseController: WidgetControllerTemplate<Widget>
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
	
	override void GetSelection(out TypeConverter type_converter)
	{
		// Since SetSelection is being used. This needs to override to avoid errors
	}
	
	override void Insert(TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			m_Widget.AddChild(type_converter.GetWidget());
		}
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			m_Widget.RemoveChild(type_converter.GetWidget());
		}
	}
	
	override void Clear()
	{
		Widget widget_a = m_Widget.GetChildren();
		while (widget_a) {
			m_Widget.RemoveChild(widget_a);
			widget_a = widget_a.GetSibling();
		}
	}

	override int Find(TypeConverter type_converter)
	{
		Widget widget_a = m_Widget.GetChildren();
		int result;
		while (widget_a) {
			if (widget_a == type_converter.GetWidget()) {
				return result;
			}

			widget_a = widget_a.GetSibling();
			result++;
		}

		return -1;
	}

	override int Count()
	{
		Widget widget_a = m_Widget.GetChildren();
		int result;
		while (widget_a) {
			widget_a = widget_a.GetSibling();
			result++;
		}

		return result;
	}
}