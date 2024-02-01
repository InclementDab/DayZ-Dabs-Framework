// Struct that just contains information of a Property
class PropertyInfo
{	
	string Name;
	typename Type;
		
	void PropertyInfo(string name, typename type)
	{
		Name = name;
		Type = type;
	}
	
	static PropertyInfo Create(Class context, string name)
	{
		if (!context) {
			return null;
		}

		return Create(context.Type(), name);
	}
	
	static PropertyInfo Create(typename parent_type, string name)
	{
		PropertyTypeHashMap hash_map = new PropertyTypeHashMap(parent_type);
		if (!hash_map[name]) {
			return null;
		}
		
		return new PropertyInfo(name, hash_map[name]);
	}
	
	// 0: Context of Start Scope, out is context of final scope
	// 1: Name of variable string Ex: m_Binding.Value.Root
	// return: Final variable name
	static PropertyInfo GetSubScope(out Class context, string name)
	{
		if (name == string.Empty) {
			return null;
		}
	
		TStringArray variable_scope = {};
		name.Split(".", variable_scope);
	
		for (int i = 0; i < variable_scope.Count() - 1; i++) {
			EnScript.GetClassVar(context, variable_scope[i], 0, context);
		}
	
		if (variable_scope.Count() == 1) {
			return PropertyInfo.Create(context, name);
		}
	
		return PropertyInfo.Create(context, variable_scope[variable_scope.Count() - 1]);
	}
}