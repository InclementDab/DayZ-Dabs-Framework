class MultilineEditBoxWidgetController : WidgetControllerTemplate<MultilineEditBoxWidget>
{
	override bool CanTwoWayBind()
	{
		return true;
	}

	override void Set(TypeConverter type_converter)
	{
		m_Widget.SetText(type_converter.GetString());
	}

	override void Get(out TypeConverter type_converter)
	{
		string out_text;
		m_Widget.GetText(out_text);
		type_converter.SetString(out_text);
	}

	override void Insert(TypeConverter type_converter)
	{
		m_Widget.SetLine(m_Widget.GetLinesCount(), type_converter.GetString());
	}
	
	override void InsertAt(int index, TypeConverter type_converter)
	{
		m_Widget.SetLine(index, type_converter.GetString());
	}

	override void Replace(int index, TypeConverter type_converter)
	{
		InsertAt(index, type_converter);
	}

	override void Remove(int index, TypeConverter type_converter)
	{
		m_Widget.SetLine(index, string.Empty);
	}

	override void Clear()
	{
		for (int i = 0; i < m_Widget.GetLinesCount(); i++) {
			m_Widget.SetLine(i, string.Empty);
		}
	}
}