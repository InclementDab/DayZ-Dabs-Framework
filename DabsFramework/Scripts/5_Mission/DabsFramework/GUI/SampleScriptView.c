
// Check JM/CF/GUI/layouts/sample_LayoutBindingManager2.layout
class SampleScriptViewController: Controller
{
	Widget content_host;
	ref ObservableCollection<ref SampleScriptView> script_view = new ObservableCollection<ref SampleScriptView>(this);
}

class SampleRelayCommand: RelayCommand
{
	override bool Execute(Class sender, CommandArgs args)
	{ 
		Print("SampleRelayCommand Execute!");
		SampleScriptViewController controller = SampleScriptViewController.Cast(m_Controller);
				
		ButtonCommandArgs commandArgs = ButtonCommandArgs.Cast(args);
		if (commandArgs.GetButtonState()) {
			controller.script_view.Insert(new SampleScriptView());
		} else {
			delete controller.script_view;
			controller.script_view = new ObservableCollection<ref SampleScriptView>(controller);
		}
		
		return true; // We dont want to keep calling up the Execution tree
	}
}


// This is just a wrapper for "DabsFramework/GUI/layouts/sample_mvc.layout"
class SampleScriptView: ScriptViewTemplate<SampleScriptViewController>
{
	override string GetLayoutFile() 
	{
		return "DabsFramework/GUI/layouts/sample_mvc.layout";
	}
	
	// not used since we used ScriptViewTemplate 
	//override typename GetControllerType();
}