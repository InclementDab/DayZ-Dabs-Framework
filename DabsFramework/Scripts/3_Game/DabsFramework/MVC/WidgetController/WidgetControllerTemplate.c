class WidgetControllerTemplate<Class T> : WidgetController
{
	protected T m_Widget;

	void SetWidget(T w)
	{
		Class.CastTo(m_Widget, w);
	}
}