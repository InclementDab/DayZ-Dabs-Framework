// Gets typename from Templated type
// This is useful when getting this error...
// 'Can't get typename from forward declaration 'Class'. Use Type("name") instead....'
class TemplateType<Class T>
{
	T value;

	static typename GetType()
	{
		return (new TemplateType<T>()).Type().GetVariableType(0);
	}
}

Widget GetChildAtIndex(Widget widget, int index)
{
	Widget result = widget.GetChildren();
	while (index > 0)
	{
		result = result.GetSibling();
		index--;
	}

	return result;
}

bool IsWidgetChild(Widget widget, Widget child)
{
	Widget result = widget.GetChildren();
	while (result) {
		if (result == child) {
			return true;
		}
		
		result = result.GetSibling();
	}

	return false;
}