class OptionSelectorColorViewController: ViewController
{	
	int Value;
	
	int Alpha, Red, Green, Blue;
	string AlphaUserInput, RedUserInput, GreenUserInput, BlueUserInput;
	float Hue, Saturation, Var;
	
	ScriptCaller OnValueChanged;
	
	override void PropertyChanged(string property_name)
	{
		Alpha 	= Math.Clamp(Alpha, 0, 255);
		Red 	= Math.Clamp(Red, 0, 255);
		Green 	= Math.Clamp(Green, 0, 255);
		Blue 	= Math.Clamp(Blue, 0, 255);
		
		Hue 	= Math.Clamp(Hue, 0, 360);
		Saturation = Math.Clamp(Saturation, 0, 100);
		Var 	= Math.Clamp(Var, 0, 100);
		
		switch (property_name) {
			case "Alpha":		
			case "Red":
			case "Green":
			case "Blue": {
				Value = ARGB(Alpha, Red, Green, Blue);		
				NotifyPropertyChanged("Value");
				break;
			}
			
			case "Hue": 
			case "Saturation": 
			case "Var": {
				Value = HSVtoARGB(Hue, Saturation, Var, Alpha);				
				NotifyPropertyChanged("Value");
				break;
			}
			
			case "Value": {
				InverseARGB(Value, Alpha, Red, Green, Blue);
				RGBtoHSV(Red, Green, Blue, Hue, Saturation, Var);
				
				AlphaUserInput = Alpha.ToString();
				RedUserInput = Red.ToString();
				GreenUserInput = Green.ToString();
				BlueUserInput = Blue.ToString();
								
				NotifyPropertiesChanged({"Alpha", "Red", "Green", "Blue", "Hue", "Saturation", "Var", "AlphaUserInput", "RedUserInput", "GreenUserInput", "BlueUserInput"}, false);
				
				if (OnValueChanged) {
					OnValueChanged.Invoke();
				}
				
				break;
			}
			
			// Force user input to be alpha numeric
			case "AlphaUserInput": 
			case "RedUserInput":
			case "GreenUserInput":
			case "BlueUserInput": {
				EnScript.SetClassVar(this, property_name, 0, Math.Clamp(EnScriptVar<string>.Get(this, property_name).ToInt(), 0, 255).ToString());
				NotifyPropertyChanged(property_name, false);
				break;
			}
		
		}		
	}
}