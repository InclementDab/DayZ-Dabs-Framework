
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


	Quick overrides:
		override bool OnClick(Widget w, int x, int y, int button);
		override bool OnModalResult(Widget w, int x, int y, int code, int result);
		override bool OnDoubleClick(Widget w, int x, int y, int button);
		override bool OnSelect(Widget w, int x, int y);
		override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn);
		override bool OnFocus(Widget w, int x, int y);
		override bool OnFocusLost(Widget w, int x, int y);
		override bool OnMouseEnter(Widget w, int x, int y);
		override bool OnMouseLeave(Widget w, Widget enterW, int x, int y);
		override bool OnMouseWheel(Widget w, int x, int y, int wheel);
		override bool OnMouseButtonDown(Widget w, int x, int y, int button);
		override bool OnMouseButtonUp(Widget w, int x, int y, int button);
		override bool OnController(Widget w, int control, int value);
		override bool OnKeyDown(Widget w, int x, int y, int key);
		override bool OnKeyUp(Widget w, int x, int y, int key);
		override bool OnKeyPress(Widget w, int x, int y, int key);
		override bool OnChange(Widget w, int x, int y, bool finished);
		override bool OnDrag(Widget w, int x, int y);
		override bool OnDragging(Widget w, int x, int y, Widget reciever);
		override bool OnDraggingOver(Widget w, int x, int y, Widget reciever);
		override bool OnDrop(Widget w, int x, int y, Widget reciever);
		override bool OnDropReceived(Widget w, int x, int y, Widget reciever);
		override bool OnResize(Widget w, int x, int y);
		override bool OnChildAdd(Widget w, Widget child);
		override bool OnChildRemove(Widget w, Widget child);
		override bool OnUpdate(Widget w);

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

#ifndef WORKBENCH
#ifndef NO_GUI
		m_LayoutRoot = CreateWidget(null);
#endif
#endif
		
		LoadWidgetsAsVariables(this, new PropertyTypeHashMap(Type()), m_LayoutRoot);

#ifndef WORKBENCH
#ifndef NO_GUI
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
			LoadWidgetsAsVariables(m_Controller, new PropertyTypeHashMap(GetControllerType()), m_LayoutRoot);
			
			// ViewController controls the hierarchy events
			m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		}
		
		m_Controller.SetParent(this);
		m_LayoutRoot.SetUserData(this);	
		
		GetGame().GetUpdateQueue(CALL_CATEGORY_SYSTEM).Insert(Update);
#endif
#endif
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

		WorkspaceWidget workspace = GetGame().GetWorkspace();
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
	
	// Safest way to delete from inside scriptview. not really needed outside
	protected void Delete()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(_delete);
	}
	
	private void _delete()
	{
		delete this;
	}
	
	// Loads .layout file Widgets into Properties of context (when they are the same name)
	/*
	Example:
	
	.layout file:
	MenuBarRoot		FrameWidget
		MenuBarFile   	ButtonWidget
			MenuBarFileLabel	TextWidget
	
	
	.c file:
	class TestClass
	{
		ButtonWidget MenuBarFile; //<-- these properties will be assigned
		private TextWidget MenuBarFileLabel;
	}
	*/
		
	static void LoadWidgetsAsVariables(Class context, PropertyTypeHashMap property_map, Widget root_widget)
	{
		if (!root_widget) {
			return;
		}
		
		foreach (string property_name, typename property_type: property_map) {
			if (!property_type.IsInherited(Widget)) {
				continue;
			}
	
			string property_name_formatted = property_name;
			property_name_formatted.Replace(".", "");	
			Widget target = root_widget.FindAnyWidget(property_name_formatted);
	
			// fixes bug that breaks everything
			if (target && root_widget.GetName() != property_name_formatted) {
				EnScript.SetClassVar(context, property_name_formatted, 0, target);
				continue;
			}
	
			// Allows you to define the layout root aswell within it
			if (!target && root_widget.GetName() == property_name_formatted) {
				EnScript.SetClassVar(context, property_name_formatted, 0, root_widget);
				continue;
			}
		}
	}
	
	// Working on my own standard here
	
	// 	Button
	//		Button.Icon
	//			Button.Text
	
	// calling FindWidetClass(Button.Icon, "Text") returns Button.Text
	
	static Widget FindWidgetClass(notnull Widget parent, string classname)
	{
		string name = parent.GetName();
		array<string> name_split = {};
		name.Split(".", name_split);
		if (name_split.Count() > 1) {
			name = name_split[0];
		}
				
		return parent.FindAnyWidget(string.Format("%1.%2", name, classname));	
	}
}