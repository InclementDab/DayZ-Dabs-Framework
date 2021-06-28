class ComplexPrefabBase<Class TController, Class TValue>:ScriptView
{
	protected TController<TValue> m_PrefabBaseController;
	protected Class m_BindingContext;
	protected string m_BindingName;
	
	// With Direct Binding, I think we can depreciate the default_value
	void ComplexPrefabBase(string caption, Class binding_context, string binding_name)
	{
		if (!TController.IsInherited(ViewController)) {
			Error("TController value must inherit from ViewController with a Template Parameter");
			return;
		}
		
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
	
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

	override typename GetControllerType() 
	{
		return (new TController<TValue>()).Type();
	}
	
	TController<TValue> GetPrefabController() 
	{
		return m_PrefabBaseController;
	}
}

