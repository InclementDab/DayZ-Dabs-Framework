ref DialogCategoryBase m_DialogCategoryBase;

void RunDialogTest()
{
	m_DialogCategoryBase = new DialogCategoryBase("Test");
	
	GroupPrefab group1 = new GroupPrefab("Group1", null, string.Empty);
	group1.Insert(new SliderPrefab("Slider1", null, ""));
	
	GroupPrefab group2 = new GroupPrefab("Group2", null, string.Empty);
	group2.Insert(new SliderPrefab("Slider2", null, ""));
	
	array<ref ScriptView> arry = { group1, group2 };
	m_DialogCategoryBase.AddContent("Testing1", arry);
	
	GroupPrefab group3 = new GroupPrefab("group3", null, string.Empty);
	group3.Insert(new EditBoxPrefab("Edit", null, ""));
	
	GroupPrefab group4 = new GroupPrefab("group4", null, string.Empty);
	group4.Insert(new CheckBoxPrefab("Check", null, ""));

	array<ref ScriptView> arry2 = { group3, group4 };
	m_DialogCategoryBase.AddContent("Testing2", arry2);
	
	DialogResult result = m_DialogCategoryBase.ShowDialog();
}


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
		foreach (ScriptView script_view: content) {
			category_list_item.AddContent(script_view);
			script_view.SetParent(this);
			script_view.GetLayoutRoot().Show(false);
			m_DialogCategoryBaseController.DialogContentData.Insert(script_view);
		}
		
		category_list_item.SetParent(this);
		m_DialogCategoryBaseController.DialogCategoryData.Insert(category_list_item);
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

class DialogCategoryListItemController: ViewController
{
	string Icon;
	string Caption;
}

class DialogCategoryListItem: ScriptViewTemplate<DialogCategoryListItemController>
{
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