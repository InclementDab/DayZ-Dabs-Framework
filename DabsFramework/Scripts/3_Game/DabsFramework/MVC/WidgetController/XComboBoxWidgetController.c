class XComboBoxWidgetController : WidgetControllerTemplate<XComboBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter type_converter)
	{
		m_Widget.SetCurrentItem(type_converter.GetInt());
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		type_converter.SetInt(m_Widget.GetCurrentItem());
	}

	override void Insert(TypeConverter type_converter)
	{
		m_Widget.AddItem(type_converter.GetString());
	}

	override void InsertAt(int index, TypeConverter type_converter)
	{
		m_Widget.SetItem(index, type_converter.GetString());
	}

	override void Replace(int index, TypeConverter type_converter)
	{
		InsertAt(index, type_converter);
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		m_Widget.RemoveItem(index);
	}

	override void Clear()
	{
		m_Widget.ClearAll();
	}

	override int Count()
	{
		return m_Widget.GetNumItems();
	}
}