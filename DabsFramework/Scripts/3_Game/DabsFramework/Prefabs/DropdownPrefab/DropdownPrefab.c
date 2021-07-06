class DropdownListPrefab<Class TValue>: ScriptView
{	
	private static const TValue EMPTY_VALUE;
	
	protected TValue m_DefaultValue;
	
	protected DropdownListPrefabController<TValue> m_DropdownPrefabController;
	
	protected ref array<ref DropdownListPrefabItem<TValue>> m_ItemList = {};
	
	protected bool m_IsListVisible;
	
	protected Class m_BindingContext;
	protected string m_BindingName;
	
	void DropdownListPrefab(string caption, Class binding_context, string binding_name)
	{
		m_BindingName = binding_name;
		m_BindingContext = binding_context;
		
		Class.CastTo(m_DropdownPrefabController, m_Controller);
		m_DropdownPrefabController.Caption = caption;
		m_DropdownPrefabController.NotifyPropertyChanged("Caption", false);
		
		EnScript.GetClassVar(m_BindingContext, m_BindingName, 0, m_DefaultValue);
	}
	
	void ~DropdownListPrefab()
	{
		delete m_ItemList;
	}
	
	void Set(string item_text, TValue user_data)
	{
		DropdownListPrefabItem<TValue> element = new DropdownListPrefabItem<TValue>(item_text, user_data);
		element.SetParent(this);			

		// Assign default item when you find it (cant assign something that doesnt exist)
		if (user_data == m_DefaultValue) {
			SetActiveListItem(element);
		}
		
		element.GetLayoutRoot().Show(false);
		m_ItemList.Insert(element);
	}
			
	bool DropdownPrefabExecute(ButtonCommandArgs args)
	{
		ShowList(!m_IsListVisible);
		return true;
	}
	
	bool DropdownElementExecute(DropdownListPrefabItemBase item)
	{
		SetActiveListItem(item);
		ShowList(false);
		return true;
	}
	
	void SetActiveListItem(DropdownListPrefabItemBase item)
	{
		m_DropdownPrefabController.Value = item;
		m_DropdownPrefabController.NotifyPropertyChanged("Value");
	}
	
	DropdownListPrefabItem<TValue> GetListItem(TValue value)
	{
		foreach (DropdownListPrefabItem<TValue> list_item: m_ItemList) {
			if (list_item.GetValue() == value) {
				return list_item;
			}
		}
				
		return null;
	}
	
	void ShowList(bool state)
	{
		m_IsListVisible = state;

		for (int i = 0; i < m_ItemList.Count(); i++) {
			float s_x, s_y, s_l, s_h, p_x, p_y, p_h, p_l;
			GetLayoutRoot().GetScreenPos(s_x, s_y);
			GetLayoutRoot().FindAnyWidget("DropdownPrefabButton").GetScreenPos(p_x, p_y);
			GetLayoutRoot().FindAnyWidget("DropdownPrefabButton").GetScreenSize(p_l, p_h);
			m_ItemList[i].GetLayoutRoot().GetScreenSize(s_l, s_h);
			m_ItemList[i].GetLayoutRoot().SetPos(p_x, p_y + s_h + (s_h * i));
			m_ItemList[i].GetLayoutRoot().Show(state);
		}
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