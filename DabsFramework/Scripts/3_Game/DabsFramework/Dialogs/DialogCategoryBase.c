ref DialogCategoryBase m_DialogCategoryBase;

void RunDialogTest()
{
	m_DialogCategoryBase = new DialogCategoryBase("Test");
	
	GroupPrefab group1 = new GroupPrefab("Group1", null, string.Empty);
	group1.Insert(new SliderPrefab("Slider1", null, ""));
	m_DialogCategoryBase.AddContent("Testing1", group1);
	
	GroupPrefab group2 = new GroupPrefab("Group2", null, string.Empty);
	group2.Insert(new SliderPrefab("Slider2", null, ""));
	m_DialogCategoryBase.AddContent("Testing2", group2);
	
	DialogResult result = m_DialogCategoryBase.ShowDialog();
}


class DialogCategoryBaseController: DialogBaseController
{
	ref ObservableCollection<ref DialogCategoryListItem> DialogCategoryData = new ObservableCollection<ref DialogCategoryListItem>(this);		

	ScriptView CurrentContent;
		
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
	
	ScriptView AddContent(string category, ScriptView content)
	{
		content.SetParent(this);
		
		m_DialogCategoryBaseController.DialogCategoryData.Insert(new DialogCategoryListItem(category, content, this));
		
		//m_DialogCategoryBaseController.DialogContentData.Insert(content);
		return content;
	}
	
	void SetActiveCategory(DialogCategoryListItem list_item)
	{
		m_DialogCategoryBaseController.CurrentContent = list_item.GetContent();
		m_DialogCategoryBaseController.NotifyPropertyChanged("CurrentContent");
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
	protected ScriptView m_Content;
	
	void DialogCategoryListItem(string caption, ScriptView content, DialogCategoryBase parent)
	{
		m_Content = content;
		m_DialogCategoryBase = parent;
		
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
	
	ScriptView GetContent()
	{
		return m_Content;
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/ListItems/CategorizedListItem.layout";
	}
}