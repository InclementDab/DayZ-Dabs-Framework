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
	
	static PropertyInfo GetFromClass(Class context, string name)
	{
		if (!context) {
			return null;
		}

		PropertyTypeHashMap hash_map = PropertyTypeHashMap.FromType(context.Type());
		if (hash_map[name]) {
			return new PropertyInfo(name, hash_map[name]);
		}
		
		return null;
	}
	
	static PropertyInfo GetFromType(typename parent_type, string name)
	{
		PropertyTypeHashMap hash_map = PropertyTypeHashMap.FromType(parent_type);
		if (hash_map[name]) {
			return new PropertyInfo(name, hash_map[name]);
		}
		
		return null;
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
			return PropertyInfo.GetFromClass(context, name);
		}
	
		return PropertyInfo.GetFromClass(context, variable_scope[variable_scope.Count() - 1]);
	}
}