
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
	protected ref ViewController m_Controller;
	ViewController GetController()
	{
		return m_Controller;
	}

	// Maybe one day we'll get constructor overloading :)
	void ScriptView()
	{
		m_LayoutRoot = CreateWidget(null);

		LoadViewProperties(this, new PropertyTypeHashMap(Type()), m_LayoutRoot);

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
			LoadViewProperties(m_Controller, new PropertyTypeHashMap(GetControllerType()), m_LayoutRoot);
			m_Controller.OnWidgetScriptInit(m_LayoutRoot);
		}

		m_Controller.Debug_Logging = Debug_Logging;
		m_Controller.SetParent(this);
		m_LayoutRoot.SetUserData(this);
		//m_LayoutRoot.SetHandler(this);
	}

	void ~ScriptView()
	{
		delete m_Controller;

		if (m_LayoutRoot) {
			Log("~" + m_LayoutRoot.GetName());
			m_LayoutRoot.Unlink();
		}
	}

	void SetParent(Widget parent)
	{
		m_LayoutRoot = CreateWidget(parent);
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
	
	static void LoadViewProperties(Class context, PropertyTypeHashMap property_map, Widget root_widget)
	{
		foreach (string property_name, typename property_type: property_map) {
			if (!property_type.IsInherited(Widget)) {
				continue;
			}
	
			Widget target = root_widget.FindAnyWidget(property_name);
	
			// fixes bug that breaks everything
			if (target && root_widget.GetName() != property_name) {
				EnScript.SetClassVar(context, property_name, 0, target);
				continue;
			}
	
			// Allows you to define the layout root aswell within it
			if (!target && root_widget.GetName() == property_name) {
				EnScript.SetClassVar(context, property_name, 0, root_widget);
				continue;
			}
		}
	}

	// Virtual Methods
	protected string GetLayoutFile();

	protected typename GetControllerType()
	{
		return ViewController;
	}
}