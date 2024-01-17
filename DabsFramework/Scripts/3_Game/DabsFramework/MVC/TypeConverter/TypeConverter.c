// Used for Converting Data within GetDayZGame().
// Register your own with GetDayZGame().RegisterConversionTemplates
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

	ScriptCaller GetterFromType(typename type)
	{
		switch (type) {
			case bool:
				return ScriptCaller.Create(GetBool);
			case int:
				return ScriptCaller.Create(GetInt);
			case float:
				return ScriptCaller.Create(GetFloat);
			case string:
				return ScriptCaller.Create(GetString);
			case vector:
				return ScriptCaller.Create(GetVector);
			case Widget:
				return ScriptCaller.Create(GetWidget);
			case Object:
				return ScriptCaller.Create(GetObject);
			case Param:
				return ScriptCaller.Create(GetParam);
		}

		return null;
	}

	ScriptCaller SetterFromType(typename type)
	{
		switch (type) {
			case bool:
				return ScriptCaller.Create(SetBool);
			case int:
				return ScriptCaller.Create(SetInt);
			case float:
				return ScriptCaller.Create(SetFloat);
			case string:
				return ScriptCaller.Create(SetString);
			case vector:
				return ScriptCaller.Create(SetVector);
			case Widget:
				return ScriptCaller.Create(SetWidget);
			case Object:
				return ScriptCaller.Create(SetObject);
			case Param:
				return ScriptCaller.Create(SetParam);
		}

		return null;
	}
}

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
		PropertyInfo property_info = PropertyInfo.GetSubScope(context, name);
		EnScript.SetClassVar(context, property_info.Name, index, m_Value);
	}

	override void GetFromController(Class context, string name, int index)
	{
		PropertyInfo property_info = PropertyInfo.GetSubScope(context, name);
		EnScript.GetClassVar(context, property_info.Name, index, m_Value);
	}
}