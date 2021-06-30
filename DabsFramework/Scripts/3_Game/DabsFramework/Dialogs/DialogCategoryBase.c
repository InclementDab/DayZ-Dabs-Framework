class DialogCategoryBaseController: DialogBaseController
{
	ref ObservableCollection<ref DialogCategoryListItem> DialogCategoryData = new ObservableCollection<ref DialogCategoryListItem>(this);		
		
	void ~DialogCategoryBaseController()
	{
		delete DialogCategoryData;
	}
}

class DialogCategoryBase: DialogBase
{
	protected DialogCategoryBaseController m_DialogCategoryBaseController;
	
	void DialogCategoryBase(string title)
	{
		m_DialogCategoryBaseController = DialogCategoryBaseController.Cast(m_Controller);
	}
	
	void AddContent(string caption, array<ref ScriptView> content)
	{
		DialogCategoryListItem category_list_item = new DialogCategoryListItem(caption);
		category_list_item.SetContent(content);
		AddContent(category_list_item);
	}
	
	void AddContent(DialogCategoryListItem category_list_item)
	{
		array<ref ScriptView> content = category_list_item.GetContent();
		foreach (ScriptView script_view: content) {
			script_view.SetParent(this);
			script_view.GetLayoutRoot().Show(false);
			m_DialogCategoryBaseController.DialogContentData.Insert(script_view);
		}
		
		category_list_item.SetParent(this);
		m_DialogCategoryBaseController.DialogCategoryData.Insert(category_list_item);
	}
	
	void SetActiveCategory(DialogCategoryListItem list_item)
	{
		array<ref ScriptView> set_content = list_item.GetContent();
		for (int i = 0; i < m_DialogCategoryBaseController.DialogContentData.Count(); i++) {
			m_DialogCategoryBaseController.DialogContentData[i].GetLayoutRoot().Show(set_content.Find(m_DialogCategoryBaseController.DialogContentData[i]) != -1);
		}
	}
	
	override typename GetControllerType() 
	{
		return DialogCategoryBaseController;
	}	
	
	override string GetLayoutFile()
	{
		return "DabsFramework/GUI/layouts/dialogs/DialogCategoryBase.layout";
	}
}