
// Gets typename from Templated type
// This is useful when getting this error...
// 'Can't get typename from forward declaration 'Class'. Use Type("name") instead....'
class TemplateType<Class T>: Managed
{
	private void TemplateType();
	private void ~TemplateType();

	T value;

	static typename GetType()
	{
		TemplateType<T> type = new TemplateType<T>();
		typename type_value = type.Type().GetVariableType(0);
		delete type;
		return type_value;
	}
}