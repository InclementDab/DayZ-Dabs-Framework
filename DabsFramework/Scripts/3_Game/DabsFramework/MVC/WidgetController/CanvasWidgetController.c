class CanvasWidgetController: WidgetControllerTemplate<CanvasWidget>
{
	override void Set(TypeConverter type_converter)
	{
		float x_pos, y_pos, x_size, y_size;
		m_Widget.GetScreenPos(x_pos, y_pos);
		m_Widget.GetScreenSize(x_size, y_size);
		
		float x_center = x_pos + (x_size / 2);
		
		m_Widget.DrawLine(x_center, y_pos, x_center, y_pos - y_size, x_size, type_converter.GetInt());
	}
}