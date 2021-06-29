ref DialogCategoryBase m_DialogCategoryBase;

void RunDialogTest()
{
	m_DialogCategoryBase = new DialogCategoryBase("Test");
	m_DialogCategoryBase.AddContent("Testing", new SliderPrefab("Slider", null, ""));
	m_DialogCategoryBase.AddContent("Testing", new SliderPrefab("Slider", null, ""));
	DialogResult result = m_DialogCategoryBase.ShowDialog();
	Print(result);
}


class DialogCategoryBaseController: DialogBaseController
{
	ref map<ScriptView, ScriptView> DialogCategoryMap = new map<ScriptView, ScriptView>();
	
	ref ObservableCollection<ref DialogCategoryListItem> DialogCategoryData = new ObservableCollection<ref DialogCategoryListItem>(this);		

	ScriptView CurrentContent;
		
	void ~DialogCategoryBaseController()
	{
		delete DialogCategoryMap;
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
		
		m_DialogCategoryBaseController.DialogCategoryData.Insert(new DialogCategoryListItem(category));
		
		m_DialogCategoryBaseController.DialogContentData.Insert(content);
		return content;
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
	protected ScriptView m_Content;
	
	void DialogCategoryListItem(string caption, ScriptView content)
	{
		m_Content = content;
		
		m_TemplateController.Caption = caption;
		m_TemplateController.NotifyPropertyChanged("Caption");
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