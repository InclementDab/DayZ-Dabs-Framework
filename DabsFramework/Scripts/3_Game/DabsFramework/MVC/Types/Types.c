// 0: Property Name
// 1: Property Type
class PropertyTypeHashMap: map<string, typename>
{
	static PropertyTypeHashMap FromType(typename type)
	{
		PropertyTypeHashMap hash_map = new PropertyTypeHashMap();
		for (int i = 0; i < type.GetVariableCount(); i++) {
			hash_map.Insert(type.GetVariableName(i), type.GetVariableType(i));	
		}
		
		return hash_map;
	}
	
	void RemoveType(typename removed_type)
	{
		PropertyTypeHashMap hash_map = FromType(removed_type);
		foreach (string name, typename type: hash_map) {
			Remove(name);
		}
	}
}


// 0: Source Widget
// 1: View Binding
typedef map<Widget, ViewBinding> ViewBindingHashMap;

// 0: View Binding
typedef set<ViewBinding> ViewBindingArray;

// 0: Property Name
// 1: View Binding Set
class DataBindingHashMap: map<string, autoptr ViewBindingArray>
{
	void DebugPrint()
	{
		foreach (string name, ViewBindingArray view_set: this) {
			MVC.Log("[%1]:", name);
			foreach (ViewBinding view: view_set) {
				MVC.Log("    %1", view.GetLayoutRoot().GetName());
			}
		}
	}
	
	void InsertView(ViewBinding view)
	{
		ViewBindingArray view_set = Get(view.Binding_Name);
		if (view_set) {
			view_set.Insert(view);
			return;
		} 
		
		view_set = new ViewBindingArray();
		view_set.Insert(view);
		Insert(view.Binding_Name, view_set);
		Insert(view.Selected_Item, view_set);
	}
}

// 0: Relay_Command parameter
// 1: Command Value
typedef map<string, RelayCommand> RelayCommandHashMap

// 0: Source Type
// 1: Conversion Type
typedef map<typename, typename> TypenameHashMap;

// 0: Source Type
// 1: Conversion Type
class TypeConversionHashMap
{
	protected ref map<typename, ref TypeConverter> m_Values = new map<typename, ref TypeConverter>();
		
	void ~TypeConversionHashMap()
	{
		delete m_Values;
	}
	
	TypeConverter Get(typename conversion_type)
	{
		TypeConverter result = m_Values.Get(conversion_type);
		
		if (!result) {
			foreach (typename type, TypeConverter conversion: m_Values) {
				if (conversion_type.IsInherited(type)) {
					return conversion;
				}
			}
		}
		
		return result;
	}
	
	void Remove(typename conversion_type) 
	{
		m_Values.Remove(conversion_type);
	}
	
	void Set(typename conversion_type, typename conversion_class) 
	{
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversion_class.ToString()));
			return;
		}
		
		TypeConverter converter = TypeConverter.Cast(conversion_class.Spawn());
		if (!converter) {
			MVC.Error(string.Format("TypeConverterHashMap: Failed to create TypeConverter %1", conversion_class));
			return;
		}
		
		m_Values.Set(conversion_type, converter);
	} 
	
	bool Insert(typename conversion_type, typename conversion_class)
	{
		if (!conversion_class.IsInherited(TypeConversionTemplate)) {
			MVC.Error(string.Format("TypeConverterHashMap: %1 must inherit from type TypeConversionTemplate", conversion_class.ToString()));
			return false;
		}
		
		TypeConverter converter = TypeConverter.Cast(conversion_class.Spawn());
		if (!converter) {
			MVC.Error(string.Format("TypeConverterHashMap: Failed to create TypeConverter %1", conversion_class));
			return false;
		}
		
		return m_Values.Insert(conversion_type, converter);
	}
}