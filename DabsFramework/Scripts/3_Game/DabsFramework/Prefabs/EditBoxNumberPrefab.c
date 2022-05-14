class EditBoxNumberPrefab: PrefabBase<StringEvaluater>
{
	protected float m_StepSize;
	
	EditBoxWidget ContentText;
	
	void EditBoxNumberPrefab(string caption, Class binding_context, string binding_name, float step_size = 1)
	{
		m_StepSize = step_size;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel;
		if (m_BindingVariableType == float) {
			motion *= 0.5;
		}
		
		motion *= m_StepSize;
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			motion *= 10;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			motion *= 0.1;
		}
		
		switch (w.GetName()) {
			case "ContentText": {
				m_PrefabBaseController.Value = string.ToString(m_PrefabBaseController.Value.Parse() + motion);
				m_PrefabBaseController.NotifyPropertyChanged("Value");
				break;
			}
		}
		
		return true;
	}
	
	override StringEvaluater GetDefaultValue(Class binding_context, string binding_name)
	{
		switch (m_BindingVariableType) {
			case float: {
				float float_value;
				EnScript.GetClassVar(binding_context, binding_name, 0, float_value);
				return float_value.ToString();
			}
			
			case int: {
				int int_value;
				EnScript.GetClassVar(binding_context, binding_name, 0, int_value);
				return int_value.ToString();
			}
		}
		
		return string.Empty;
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		float raw_value = m_PrefabBaseController.Value.Parse();
		switch (m_BindingVariableType) {
			case int: {
				// needs to be exact type, engine wont cast for us
				EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, (int)raw_value); 
				break;
			}
			
			case float: {
				EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, raw_value);
				break;
			}
			
			default: {
				Error("Invalid var type for NumberPrefab, use int or float");
				return;
			}
		}
		
		g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/EditBoxPrefab.layout";
	}
}