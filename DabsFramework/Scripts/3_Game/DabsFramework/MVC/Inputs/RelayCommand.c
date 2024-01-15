// Abstract Class
class RelayCommand: Managed
{
	protected ViewController m_Controller;
	void SetController(ViewController controller)
	{
		m_Controller = controller;
	}

	protected ViewBinding m_ViewBinding;
	void SetViewBinding(ViewBinding view_binding)
	{
		m_ViewBinding = view_binding;
	}
	
	ViewBinding GetViewBinding()
	{
		return m_ViewBinding;
	}

	// returns bool defining whether or not the Execute can be called
	bool CanExecute()
	{
		return true;
	}

	/* Abstract Methods */

	// called when type is Clicked, Selected, or Changed
	// Return: Handled (if false, will continue to execute up the tree)
	bool Execute(Class sender, CommandArgs args);
}