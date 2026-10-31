class PrefabBaseController<Class TValue>: ViewController
{
	string Caption;
	TValue Value;
	TValue CalculatedValue; // Used for things like SliderWidget output
	
	override void PropertyChanged(string property_name)
	{
		if (GetParent() && GetParent().IsInherited(PrefabBase)) {
			g_Script.CallFunction(GetParent(), "PrefabPropertyChanged", null, property_name);
		}
	}
}