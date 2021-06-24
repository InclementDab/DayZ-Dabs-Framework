class TextListboxController : WidgetControllerTemplate<TextListboxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void SetSelection(TypeConverter type_converter)
	{
		for (int i = 0; i < m_Widget.GetNumItems(); i++)
		{
			string row_text;
			m_Widget.GetItemText(i, 0, row_text);
			if (row_text == type_converter.GetString()) {
				m_Widget.SelectRow(i);
				return;
			}
		}
	}

	override void GetSelection(out TypeConverter type_converter)
	{
		string selection;
		m_Widget.GetItemText(m_Widget.GetSelectedRow(), 0, selection);
		type_converter.SetString(selection);
	}

	override void Insert(TypeConverter type_converter)
	{
		m_Widget.AddItem(type_converter.GetString(), type_converter, 0);
	}

	override void InsertAt(int index, TypeConverter type_converter)
	{
		m_Widget.SetItem(index, type_converter.GetString(), type_converter, 0);
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		m_Widget.SetItem(index, string.Empty, type_converter, 0);
	}

	override void Swap(int index_a, int index_b)
	{
		string textA, textB;
		Class dataA, dataB;

		if (index_a < 0 || index_a > m_Widget.GetNumItems() || index_b < 0 || index_b > m_Widget.GetNumItems()) {
			return;
		}

		m_Widget.GetItemText(index_a, 0, textA);
		m_Widget.GetItemData(index_a, 0, dataA);

		m_Widget.GetItemText(index_b, 0, textB);
		m_Widget.GetItemData(index_b, 0, dataB);

		m_Widget.SetItem(index_b, textA, dataA, 0);
		m_Widget.SetItem(index_a, textB, dataB, 0);
	}

	override void Clear()
	{
		m_Widget.ClearItems();
	}

	override int Count()
	{
		return m_Widget.GetNumItems();
	}
}