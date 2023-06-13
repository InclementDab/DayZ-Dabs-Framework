
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
	window.FunctionsThatOccurOnUi();
	....
	....
*/

class ScriptView: ScriptedViewBase
{
	static ref array<ScriptView> All = {};
		
	protected ref ViewController m_Controller;
	
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
		
		m_Controller.SetParent(this);
		m_LayoutRoot.SetUserData(this);	
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(Update);
	}

	void ~ScriptView()
	{
		if (m_LayoutRoot) {
			Log("~" + m_LayoutRoot.GetName());
		}
		
		//1 Remove update first
		if (GetGame()) {
			if (GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM)) {
				GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Remove(Update);
			}
		}
		
		//2 delete class instances
		delete m_Controller;

		//3 unlink our layout, deleting it
		if (m_LayoutRoot) {
			m_LayoutRoot.Unlink();
		}
		
		//4 remove from cache
		if (All) {
			All.RemoveItem(this);
		}
	}
	
	protected void Update(float dt)
	{
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
			
	// Virtual Methods
	protected string GetLayoutFile()
	{
		return string.Empty;
	}
			
	protected typename GetControllerType()
	{
		return ViewController;
	}
	
	ViewController GetController()
	{
		return m_Controller;
	}
}