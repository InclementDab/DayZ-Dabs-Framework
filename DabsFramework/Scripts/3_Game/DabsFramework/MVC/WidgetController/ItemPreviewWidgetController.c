class ItemPreviewWidgetController : WidgetControllerTemplate<ItemPreviewWidget>
{
	override void Set(TypeConverter type_converter)
	{
		EntityAI entity;
		if (Class.CastTo(entity, type_converter.GetObject())) {
			m_Widget.SetView(0);
			m_Widget.SetItem(entity);
			m_Widget.SetModelPosition(vector.Zero);
		}
	}

	override void Get(out TypeConverter type_converter)
	{
		type_converter.Set(m_Widget.GetItem());
	}
}