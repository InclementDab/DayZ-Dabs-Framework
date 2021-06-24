class HtmlWidgetController : WidgetControllerTemplate<HtmlWidget>
{
    override void Set(TypeConverter type_converter)
    {
        m_Widget.SetText(type_converter.GetString());
    }
}