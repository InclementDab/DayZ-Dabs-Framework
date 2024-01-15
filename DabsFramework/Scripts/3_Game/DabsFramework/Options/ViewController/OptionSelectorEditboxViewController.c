class OptionSelectorEditboxViewController: ViewController
{
	ScriptCaller OnValueChanged;
	
	string Value;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {			
			case "Value": {
				if (OnValueChanged) {
					OnValueChanged.Invoke();
				}
				
				break;
			}
		}
	}
}