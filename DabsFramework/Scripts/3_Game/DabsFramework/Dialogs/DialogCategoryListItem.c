class DialogCategoryListItemController: ViewController
{
	string Icon;
	string Caption;
}

class DialogCategoryListItem: ScriptViewTemplate<DialogCategoryListItemController>
{
	ButtonWidget CategorizedListItemButton;
	
	protected DialogCategoryBase m_DialogCategoryBase;
	
	protected ref array<ref ScriptView> m_Content = {};
	
	void DialogCategoryListItem(string caption)
	{
		m_TemplateController.Caption = caption;
		m_TemplateController.NotifyPropertyChanged("Caption");
	}
		
	void OnListItemExecute(ButtonCommandArgs args)
	{
		if (!m_DialogCategoryBase) {
			return;
		}
		
		m_DialogCategoryBase.SetActiveCategory(this);
	}
	
	void SetParent(DialogCategoryBase dialog)
	{
		m_DialogCategoryBase = dialog;
	}
	
	void SetContent(array<ref ScriptView> content)
	{
		m_Content = content;
	}
	
	void AddContent(ScriptView content)
	{
		m_Content.Insert(content);
	}
	
	array<ref ScriptView> GetContent()
	{
		return m_Content;
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/ListItems/CategorizedListItem.layout";
	}
}