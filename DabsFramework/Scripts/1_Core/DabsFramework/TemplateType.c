
// Gets typename from Templated type
// This is useful when getting this error...
// 'Can't get typename from forward declaration 'Class'. Use Type("name") instead....'
class TemplateType<Class T>
{
	private void TemplateType();
	private void ~TemplateType();

	T value;

	static typename GetType()
	{
		return (new TemplateType<T>()).Type().GetVariableType(0);
	}
}