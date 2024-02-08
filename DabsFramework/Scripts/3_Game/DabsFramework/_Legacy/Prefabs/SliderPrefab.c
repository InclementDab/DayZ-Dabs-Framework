class SliderPrefab: PrefabBase<float>
{
	protected float m_Min, m_Max;
	protected float m_ButtonIncrementCoef;
	
	void SliderPrefab(string caption, Class binding_context, string binding_name, float min = 0, float max = 100, float button_increment_coef = 0.1)
	{
		m_Min = min; m_Max = max; m_ButtonIncrementCoef = button_increment_coef;
		
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
				g_Script.CallFunctionParams(m_BindingContext, "PropertyChanged", null, new Param1<string>(m_BindingName));
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
		return (Math.AbsFloat(m_Max - m_Min) * m_ButtonIncrementCoef);
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/SliderPrefab.layout";
	}
}
