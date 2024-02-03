class DropdownListPrefabController<Class TValue>: ViewController
{
	string Caption;
	DropdownListPrefabItemBase Value;
	TValue CalculatedValue; // Used for things like SliderWidget output
	
	ref ObservableCollection<ref DropdownListPrefabItem<TValue>> ItemList = new ObservableCollection<ref DropdownListPrefabItem<TValue>>(this);
	
	void ~DropdownListPrefabController()
	{
		delete ItemList;
	}
	
	override void PropertyChanged(string property_name)
	{
		if (GetParent()) {
			g_Script.Call(GetParent(), "PrefabPropertyChanged", property_name);
		}
	}
}