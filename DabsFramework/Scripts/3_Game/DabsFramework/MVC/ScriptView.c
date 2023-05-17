
// Abstract Class
// Inherit from this if you want to create UI elements with 100% script
/*

Example:

class CustomDialogWindow: ScriptView
{
	override string GetLayoutFile() {
		return "MyMod/gui/Layouts/dialogs/Dialog.layout";
	}
}

	CustomDialogWindow window = new CustomDialogWindow();
	window.Show();
	....
	....
	window.Close();

*/

class ScriptView: ScriptedViewBase
{
	static ref array<ScriptView> All = {};
	
	protected ref ViewController m_Controller;
	protected ref ScriptViewMenu m_ScriptViewMenu;

	// Maybe one day we'll get constructor overloading :)
	void ScriptView()
	{
		if (!All) {
			All = {};
		}
		
		All.Insert(this);
		
		m_LayoutRoot = CreateWidget(null);

		MVC.LoadWidgetsAsVariables(this, new PropertyTypeHashMap(Type()), m_LayoutRoot);

		m_LayoutRoot.GetScript(m_Controller);

		// If no Controller is specified in the WB Root
		if (!m_Controller || !m_Controller.IsInherited(ViewController)) {

			Log("ViewController not found on %1, creating...", m_LayoutRoot.GetName());
			if (!GetControllerType().IsInherited(ViewController)) {
				Error("%1 is invalid. Must inherit from ViewController!", GetControllerType().ToString());
				return;
			}

			if (!Class.CastTo(m_Controller, GetControllerType().Spawn())) {
				Error("Could not create ViewController %1", GetControllerType().ToString());
				return;
			}

			// Since its not loaded in the WB, needs to be called here
			MVC.LoadWidgetsAsVariables(m_Controller, new PropertyTypeHashMap(GetControllerType()), m_LayoutRoot);
			
			// ViewController controls the hierarchy events
			m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		}
		
		// @ Setting up UI management, we either use a dummy referenced object or let the engine manage it depending
		if (UseUIManager()) {
			m_ScriptViewMenu = ScriptViewMenu.Cast(GetGame().GetUIManager().ShowScriptedMenu(new ScriptViewMenu(this), null));
		} else {
			m_ScriptViewMenu = new ScriptViewMenu(this);	
		}
		
		m_Controller.SetParent(this);
		m_LayoutRoot.SetUserData(this);	
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(Update);
	}

	void ~ScriptView()
	{
		if (m_LayoutRoot) {
			Log("~" + m_LayoutRoot.GetName());
		}
		
		if (GetGame()) {
			if (GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM)) {
				GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(Update);
			}
			
			if (UseUIManager()) {
				GetGame().GetUIManager().HideScriptedMenu(m_ScriptViewMenu);
			} else {
				delete m_ScriptViewMenu;
			}
		}
		
		delete m_Controller;
		
		if (m_LayoutRoot) {
			m_LayoutRoot.Unlink();
		}
		
		if (All) {
			All.RemoveItem(this);
		}
	}
	
	void Update(float dt)
	{
	}

	void SetParent(Widget parent)
	{
		if (m_LayoutRoot) {
			m_LayoutRoot.Unlink();
		}
		
		m_LayoutRoot = CreateWidget(parent);
		m_LayoutRoot.SetUserData(this);
		
		m_Controller.OnWidgetScriptInit(m_LayoutRoot);
	}

	protected Widget CreateWidget(Widget parent)
	{
		Widget result;
		if (!GetLayoutFile()) {
			Error("Layout file not found! Are you overriding GetLayoutFile?");
			return result;
		}

		WorkspaceWidget workspace = GetWorkbenchGame().GetWorkspace();
		if (!workspace) {
			Error("Workspace was null, try reloading Workbench");
			return result;
		}

		Log("Loading %1", GetLayoutFile());
		result = workspace.CreateWidgets(GetLayoutFile(), parent);
		if (!result) {
			Error("Invalid layout file %1", GetLayoutFile());
			return result;
		}

		return result;
	}

	// Useful if you want to set to an existing controller
	void SetController(ViewController controller)
	{
		m_Controller = controller;
		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		m_Controller.SetParent(this);
	}
				
	//ShowDialog(string caption, string text, int id, int butts /*DBT_*/, int def/*DBB_*/, int type /*DMT_*/, UIScriptedMenu handler)
	
	// Virtual Methods
	protected string GetLayoutFile();
	
	bool UseMouse()
	{
		return false;
	}
	
	bool UseKeyboard()
	{
		return false;
	}
	
	bool UseGamepad()
	{
		return false;
	}
	
	bool UseUIManager()
	{
		return false;
	}
	
	void OnScriptViewMenuEnter(ScriptViewMenu source);
	void OnScriptViewMenuExit(ScriptViewMenu source);
	
	protected typename GetControllerType()
	{
		return ViewController;
	}

	UIScriptedMenu GetScriptedMenu()
	{
		return m_ScriptViewMenu;
	}
	
	ViewController GetController()
	{
		return m_Controller;
	}
}