class SpacerBaseWidgetController : WidgetControllerTemplate<SpacerBaseWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			SetFocus(type_converter.GetWidget());
		}
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		if (IsWidgetChild(m_Widget, GetFocus())) {
			type_converter.SetWidget(GetFocus());
		}
	}

	override void Insert(TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			m_Widget.AddChild(type_converter.GetWidget());
		}
	}

	override void InsertAt(int index, TypeConverter type_converter)
	{
		if (index == Count()) {
			Insert(type_converter);
		} 
		
		else if (type_converter.GetWidget()) {
			Widget widget_a = GetChildAtIndex(m_Widget, index);
			m_Widget.AddChildAfter(type_converter.GetWidget(), widget_a);
		}
	}

	override void Replace(int index, TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			Widget widget_a = GetChildAtIndex(m_Widget, index);
			m_Widget.AddChildAfter(type_converter.GetWidget(), widget_a);
			m_Widget.RemoveChild(widget_a);
		}
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		if (type_converter.GetWidget()) {
			m_Widget.RemoveChild(type_converter.GetWidget());
		}
	}

	override void Move(int index, TypeConverter type_converter)
	{
		Widget widget_a = GetChildAtIndex(m_Widget, index - 1);
		if (type_converter.GetWidget() && widget_a) {
			m_Widget.RemoveChild(type_converter.GetWidget());
			m_Widget.AddChildAfter(type_converter.GetWidget(), widget_a);
		}
	}

	override void Swap(int index_a, int index_b)
	{
		if (index_a == index_b || index_a < 0 || index_b < 0) {
			return;
		}

		if (index_a > index_b) {
			int temp = index_a;
			index_a = index_b;
			index_b = temp;
		}

		Widget widget_a = GetChildAtIndex(m_Widget, index_a);
		Widget widget_b = GetChildAtIndex(m_Widget, index_b);
		Widget widget_c = GetChildAtIndex(m_Widget, index_b - 1);

		m_Widget.RemoveChild(widget_a);
		m_Widget.AddChildAfter(widget_a, widget_b);
		m_Widget.RemoveChild(widget_b);
		m_Widget.AddChildAfter(widget_b, widget_c);
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