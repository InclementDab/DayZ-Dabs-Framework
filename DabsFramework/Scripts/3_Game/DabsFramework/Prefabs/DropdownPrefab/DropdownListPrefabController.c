class DropdownListPrefabController<Class TValue>: ViewController
{
	string Caption;
	DropdownListPrefabItemBase Value;
	TValue CalculatedValue; // Used for things like SliderWidget output
		
	override void PropertyChanged(string property_name)
	{
		if (GetParent()) {
			g_Script.Call(GetParent(), "PrefabPropertyChanged", property_name);
		}
	}
}