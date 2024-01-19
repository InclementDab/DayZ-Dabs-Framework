class OptionSelectorSliderViewController: ViewController
{
	// could be unique for each setting
	static const int DECIMAL_PRECISION = 3;
	
	ScriptCaller OnValueChanged;
	
	float Min, Max, Value; // Value is 0..1
	
	float ValueActual; // real value to be set in settings / config
	
	string ValueUserInput; // value for user to interface with and attempt to input
	
	void OnUserInputFinished()
	{
		ValueActual = ValueUserInput.ToFloat();
		NotifyPropertyChanged("ValueActual");
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "Value": {
				// Update settings value
				ValueActual = Math.Lerp(Min, Max, Value);
				NotifyPropertyChanged("ValueActual", false);
				
				// Update user input visualization
				ValueUserInput = ValueActual.ToString();
				int decimal_idx = ValueUserInput.IndexOf(".");
				if (decimal_idx != -1 && ValueUserInput.Length() - decimal_idx > DECIMAL_PRECISION) {					
					ValueUserInput = ValueUserInput.SubstringInverted(ValueUserInput, decimal_idx + DECIMAL_PRECISION + 1, ValueUserInput.Length());
				}
				
				NotifyPropertyChanged("ValueUserInput", false);
				
				if (OnValueChanged) {
					OnValueChanged.Invoke();
				}
				
				break;
			}
			
			case "ValueActual": {
				ValueActual = Math.Clamp(ValueActual, Min, Max);
				Value = Math.InverseLerp(Min, Max, ValueActual);
				NotifyPropertyChanged("Value");
				break;
			}
					
			case "ValueUserInput": {
				for (int i = 0; i < ValueUserInput.Length(); i++) {
					if (!StringEvaluaterEvaluater.IsNumeric(ValueUserInput[i])) {
						ValueUserInput.Set(i, string.Empty); // enfusions broken this morning
						NotifyPropertyChanged("ValueUserInput", false);
					}
				}
				
				break;
			}
		}
	}
}