// Used for Converting Data within MVC.
// Register your own with MVC.RegisterConversionTemplates
class TypeConverter
{
	// Main Setter and Getter
	void Set(Class value)
	{
		g_Script.Call(this, "SetData", value);
	}

	Class Get()
	{
		Class result;
		g_Script.CallFunction(this, "GetData", result, null);
		return result;
	}

	typename GetType();

	bool GetBool();
	int GetInt();
	float GetFloat();
	string GetString();
	vector GetVector();
	Widget GetWidget();
	Object GetObject();
	Param GetParam();

	void SetBool(bool value);
	void SetInt(int value);
	void SetFloat(float value);
	void SetString(string value);
	void SetVector(vector value);
	void SetWidget(Widget value);
	void SetObject(Object value);
	void SetParam(Param value);

	void SetToController(Class context, string name, int index);
	void GetFromController(Class context, string name, int index);

	static func GetterFromType(typename type)
	{
		switch (type)
		{
			case bool:
				return GetBool;
			case int:
				return GetInt;
			case float:
				return GetFloat;
			case string:
				return GetString;
			case vector:
				return GetVector;
			case Widget:
				return GetWidget;
			case Object:
				return GetObject;
			case Param:
				return GetParam;
		}

		return __NULL_FUNCT;
	}

	static func SetterFromType(typename type)
	{
		switch (type)
		{
			case bool:
				return SetBool;
			case int:
				return SetInt;
			case float:
				return SetFloat;
			case string:
				return SetString;
			case vector:
				return SetVector;
			case Widget:
				return SetWidget;
			case Object:
				return SetObject;
			case Param:
				return SetParam;
		}

		return __NULL_FUNCT;
	}
};

// Inherit from THIS for creating Custom TypeConversions
class TypeConversionTemplate<Class T>: TypeConverter
{
	protected T m_Value;

	protected void SetData(T value)
	{
		m_Value = value;
	}

	protected T GetData()
	{
		return m_Value;
	}

	// This throws warnings cause casting in Enforce is stupid
	// Can't do Param1<T>.Cast(value) or Class.CastTo() because inheritence doesnt
	// work properly with Template Casting
	
	// UPDATE: Using SetClassVar is a safe way of doing this without warnings

	// This is meant to be an anonymous data setter
	// string, int, etc... dont inherit from Class :)
	
	// UPDATE 2: I could have just used the inverse and done GetClassVar the other way around
	override void SetParam(Param value)
	{
		EnScript.GetClassVar(value, "param1", 0, m_Value);
	}

	override Param GetParam()
	{
		return new Param1<T>(m_Value);
	}

	override typename GetType()
	{
		return TemplateType<T>.GetType();
	}

	override void SetToController(Class context, string name, int index)
	{
		PropertyInfo propertyInfo = GetSubScope(context, name);
		EnScript.SetClassVar(context, propertyInfo.Name, index, m_Value);
	}

	override void GetFromController(Class context, string name, int index)
	{
		PropertyInfo propertyInfo = GetSubScope(context, name);
		EnScript.GetClassVar(context, propertyInfo.Name, index, m_Value);
	}
}