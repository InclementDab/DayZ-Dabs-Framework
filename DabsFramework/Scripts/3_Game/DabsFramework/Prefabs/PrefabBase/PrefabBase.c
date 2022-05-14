class PrefabBase<Class TValue>: ScriptView
{
	protected PrefabBaseController<TValue> m_PrefabBaseController;
	protected Class m_BindingContext;
	protected string m_BindingName;
	protected typename m_BindingVariableType;
	
	void PrefabBase(string caption, Class binding_context, string binding_name)
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
		m_BindingVariableType = GetVariableTypeOfClass(m_BindingContext, m_BindingName);
	
		Class.CastTo(m_PrefabBaseController, m_Controller);
		m_PrefabBaseController.Caption = caption;
		m_PrefabBaseController.NotifyPropertyChanged("Caption", false);
		
		// Assign default value from the controller
		m_PrefabBaseController.Value = GetDefaultValue(m_BindingContext, m_BindingName);
		m_PrefabBaseController.NotifyPropertyChanged("Value", false);
	}
	
	void PrefabPropertyChanged(string property_name)
	{
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value);		
		g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
	}
		
	// Abstractable, done in SliderPrefab and EditBoxNumberPrefab
	TValue GetDefaultValue(Class binding_context, string binding_name)
	{
		TValue value;
		EnScript.GetClassVar(binding_context, binding_name, 0, value);
		return value;
	}
	
	void ResetToDefault()
	{
		EnScript.GetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.Value);
		m_PrefabBaseController.NotifyPropertyChanged("Value");
	}
	
	string GetBindingName()
	{
		return m_BindingName;
	}

	Class GetBindingContext()
	{
		return m_BindingContext;
	}
	
	override typename GetControllerType() 
	{
		return (new PrefabBaseController<TValue>()).Type();
	}
	
	PrefabBaseController<TValue> GetPrefabController() 
	{
		return m_PrefabBaseController;
	}
	
	static typename GetVariableTypeOfClass(Class cls, string variable_name) 
	{
		typename binding_ctx_type = cls.Type();
		for (int i = 0; i < binding_ctx_type.GetVariableCount(); i++) {
			if (binding_ctx_type.GetVariableName(i) == variable_name) {
				return binding_ctx_type.GetVariableType(i);
			}
		}
		
		return EMPTY_TYPENAME;
	}
}