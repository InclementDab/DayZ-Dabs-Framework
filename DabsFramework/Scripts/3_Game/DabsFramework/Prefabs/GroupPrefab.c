class GroupPrefab: PrefabBase<ObservableCollection<ref ScriptView>>
{
	protected ref ObservableCollection<ref ScriptView> m_Children;
	
	protected WrapSpacerWidget EditorDialogOptionContent;
	
	void GroupPrefab(string caption, Class binding_context, string binding_name)
	{
		m_Children = new ObservableCollection<ref ScriptView>(m_PrefabBaseController);
		m_PrefabBaseController.Value = m_Children;
	}
	
	void ~GroupPrefab()
	{
		delete m_Children;
	}
	
	void Insert(ScriptView content)
	{
		m_PrefabBaseController.Value.Insert(content);
	}
	
	bool GroupCollapseExecute(ButtonCommandArgs args)
	{
		Open(!args.GetButtonState()); 
		return true;		
	}
	
	bool IsOpen()
	{
		return EditorDialogOptionContent.IsVisible();
	}
	
	void Open(bool state)
	{
		EditorDialogOptionContent.Show(state);
	}
	
	void NotifyChildren(string property_name = "")
	{
		for (int i = 0; i < m_Children.Count(); i++) {
			m_Children[i].GetController().NotifyPropertyChanged(property_name);
		}
	}
	
	ObservableCollection<ref ScriptView> GetChildren()
	{
		return m_Children;
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/GroupPrefab.layout";
	}
}