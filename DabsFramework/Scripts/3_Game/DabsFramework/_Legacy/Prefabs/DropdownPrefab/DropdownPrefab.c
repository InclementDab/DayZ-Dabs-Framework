class DropdownListPrefab<Class TValue>: ScriptView
{	
	private static const TValue EMPTY_VALUE;
	
	protected TValue m_DefaultValue;	
	protected DropdownListPrefabController<TValue> m_DropdownPrefabController;	
	protected Class m_BindingContext;
	protected string m_BindingName;
	
	WrapSpacerWidget DropDownCollapse;
	
	void DropdownListPrefab(string caption, Class binding_context, string binding_name)
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
		
		Class.CastTo(m_DropdownPrefabController, m_Controller);
		m_DropdownPrefabController.Caption = caption;
		m_DropdownPrefabController.NotifyPropertyChanged("Caption", false);
		
		EnScript.GetClassVar(m_BindingContext, m_BindingName, 0, m_DefaultValue);
	}
			
	bool DropdownPrefabExecute(ButtonCommandArgs args)
	{
		DropDownCollapse.Show(!DropDownCollapse.IsVisible());
		return true;
	}
	
	bool DropdownElementExecute(DropdownListPrefabItemBase item)
	{
		SetActiveListItem(item);
		DropDownCollapse.Show(false);
		return true;
	}
	
	void SetActiveListItem(DropdownListPrefabItemBase item)
	{
		m_DropdownPrefabController.Value = item;
		m_DropdownPrefabController.NotifyPropertyChanged("Value");
	}
		
	void Set(string item_text, TValue user_data)
	{
		DropdownListPrefabItem<TValue> element = new DropdownListPrefabItem<TValue>(item_text, user_data);
		element.SetParent(this);			

		// Assign default item when you find it (cant assign something that doesnt exist)
		if (user_data == m_DefaultValue) {
			SetActiveListItem(element);
		}
		
		m_DropdownPrefabController.ItemList.Insert(element);
	}
	
	DropdownListPrefabItem<TValue> Get(TValue value)
	{
		for (int i = 0; i < m_DropdownPrefabController.ItemList.Count(); i++) {
			if (!m_DropdownPrefabController.ItemList[i]) {
				continue;
			}
			
			if (m_DropdownPrefabController.ItemList[i].GetValue() == value) {
				return m_DropdownPrefabController.ItemList[i];
			}
		}
				
		return null;
	}
	
	DropdownListPrefabItem<TValue> Get(string value)
	{
		for (int i = 0; i < m_DropdownPrefabController.ItemList.Count(); i++) {
			if (!m_DropdownPrefabController.ItemList[i]) {
				continue;
			}
			
			if (m_DropdownPrefabController.ItemList[i].GetText() == value) {
				return m_DropdownPrefabController.ItemList[i];
			}
		}
				
		return null;
	}
	
	void PrefabPropertyChanged(string property_name)
	{
		TValue value;
		g_Script.CallFunction(m_DropdownPrefabController.Value, "GetValue", value, null);
		EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, value);		
		g_Script.CallFunction(m_BindingContext, "PropertyChanged", null, m_BindingName);
	}
	
	override typename GetControllerType() 
	{
		return (new DropdownListPrefabController<TValue>()).Type();
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/Dropdown/DropdownPrefab.layout";
	}
}