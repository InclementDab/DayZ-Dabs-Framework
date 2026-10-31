class SliderPrefab: PrefabBase<float>
{
	protected float m_Min, m_Max;
	protected float m_ButtonIncrement, m_Resolution;
	
	void SliderPrefab(string caption, Class binding_context, string binding_name, float min = 0, float max = 1, float button_increment = 0.1, float resolution = Math.EPSILON)
	{
		m_Min = min; 
		m_Max = max; 
		m_ButtonIncrement = button_increment;
		m_Resolution = resolution;
		
		m_PrefabBaseController.CalculatedValue = GetDefaultValue(binding_context, binding_name);
		// This line will update the Value param aswell
		m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue");
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		switch (property_name) {
			case "Value": {
				m_PrefabBaseController.CalculatedValue = Math.Lerp(m_Min, m_Max, m_PrefabBaseController.Value);
				m_PrefabBaseController.CalculatedValue = Math.Clamp(m_PrefabBaseController.CalculatedValue, m_Min, m_Max);
				m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue", false);
				EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, m_PrefabBaseController.CalculatedValue);
				g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
				break;
			}
			
			case "CalculatedValue": {
				m_PrefabBaseController.Value = Math.InverseLerp(m_Min, m_Max, m_PrefabBaseController.CalculatedValue);
				m_PrefabBaseController.NotifyPropertyChanged("Value");
				break;
			}
		}
	}
	
	void OnButtonUp(ButtonCommandArgs args)
	{
		m_PrefabBaseController.CalculatedValue += GetIncrementValue();
		m_PrefabBaseController.CalculatedValue = Math.Clamp(m_PrefabBaseController.CalculatedValue, m_Min, m_Max);
		m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue");
	}
	
	void OnButtonDown(ButtonCommandArgs args)
	{
		m_PrefabBaseController.CalculatedValue -= GetIncrementValue();
		m_PrefabBaseController.CalculatedValue = Math.Clamp(m_PrefabBaseController.CalculatedValue, m_Min, m_Max);
		m_PrefabBaseController.NotifyPropertyChanged("CalculatedValue");
	}
	
	float GetIncrementValue()
	{
		return m_ButtonIncrement;
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/SliderPrefab.layout";
	}
}
