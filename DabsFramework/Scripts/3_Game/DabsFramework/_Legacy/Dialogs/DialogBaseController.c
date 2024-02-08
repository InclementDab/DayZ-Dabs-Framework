class DialogBaseController: ViewController
{
	string Title;
	string Icon;
			
	ref ObservableCollection<ref ScriptView> DialogContentData = new ObservableCollection<ref ScriptView>(this);
	ref ObservableCollection<ref DialogButton> DialogButtonData = new ObservableCollection<ref DialogButton>(this);
	
	void ~DialogBaseController()
	{		
		delete DialogContentData;
		delete DialogButtonData;
	}
}