class EnScriptVar<Class T>
{
	private void EnScriptVar();
	private void ~EnScriptVar();
	
	static T Get(Class instance, string variable_name, int index = 0)
	{
		T value;
		EnScript.GetClassVar(instance, variable_name, index, value);
		return value;
	}
	
	static void Set(Class instance, string variable_name, T value, int index = 0)
	{
		EnScript.SetClassVar(instance, variable_name, index, value);
	}
}