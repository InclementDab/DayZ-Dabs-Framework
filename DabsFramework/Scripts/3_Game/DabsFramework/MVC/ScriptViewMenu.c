//@ It is your responsibility to manage the memory for this menu object
class ScriptViewMenu: ScriptView
{		
	protected ref UIScriptViewMenu m_UIScriptViewMenu;
	
	//@ This menu will control its subsequent children UI menus, but NOT children ScriptViewMenu!!!
	protected ref UIScriptedMenu m_ChildMenu;
	
	void ScriptViewMenu()
	{
		// Hiding automatically here since we need to let the UI "Manager" do its job
		m_LayoutRoot.Show(false);
		
		m_UIScriptViewMenu = new UIScriptViewMenu(this);	
		
		GetGame().GetUIManager().ShowScriptedMenu(m_UIScriptViewMenu, null);
	}
	
	void ~ScriptViewMenu()
	{
		if (m_ChildMenu) {
			m_ChildMenu.Close();
			delete m_ChildMenu;
		}
		
		// Memory manage should lazy handle this but i'm force deleting so destructors are called in a predictable manner
		m_UIScriptViewMenu.Close();
		delete m_UIScriptViewMenu;
		
		//GetGame().GetUIManager().HideScriptedMenu(m_UIScriptViewMenu);
	}
	
	void ShowDialog(string caption, string text, int id, int buttons /*DBT_*/, int default_button /*DBB_*/, int type /*DMT_*/)
	{
		GetGame().GetUIManager().ShowDialog(caption, text, id, buttons, default_button, type, m_UIScriptViewMenu);
	}
	
	//@ important functions called by the UI manager when entering and leaving child menus
	void OnShow()
	{
		Print("OnShow");
		GetGame().GetMission().AddActiveInputExcludes(GetInputExcludes());
		
		if (UseMouse()) {
			GetGame().GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			GetGame().GetUIManager().ShowUICursor(true);
		}
		
		array<int> input_restrictions = {};
		foreach (int input_restriction: input_restrictions) {
			UAInput input = GetUApi().GetInputByID(input_restriction);
			if (!input) {
				Error(string.Format("Invalid input specified in input restrictions idx=%1", input_restriction));
				continue;
			}
			
			input.ForceEnable(true);
		}
	}
	
	void OnHide()
	{
		Print("OnHide");
		
		// Mouse control
		if (UseMouse()) {
			GetGame().GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
		}

		GetGame().GetUIManager().ShowUICursor(false); //GetParentMenu() && GetParentMenu().UseMouse()
		
		// input excludes
		GetGame().GetMission().RemoveActiveInputExcludes(GetInputExcludes());
		
		// input restrictions
		array<int> input_restrictions = {};
		foreach (int input_restriction: input_restrictions) {
			UAInput input = GetUApi().GetInputByID(input_restriction);
			if (!input) {
				continue;
			}
			
			input.ForceEnable(false);
		}
	}

	void EnterChildMenu(int id)
	{
		m_ChildMenu = GetGame().GetMission().CreateScriptedMenu(id);
		m_UIScriptViewMenu.SetSubMenu(m_ChildMenu);
	}
	
	void EnterChildMenu(UIScriptedMenu scripted_menu)
	{
		m_ChildMenu = scripted_menu;
		//m_UIScriptViewMenu.SetSubMenu(m_ChildMenu);
		GetGame().GetUIManager().ShowScriptedMenu(scripted_menu, m_UIScriptViewMenu);
	}
		
	void EnterChildMenu(ScriptViewMenu menu)
	{
		m_ChildMenu = menu.GetUIScriptViewMenu();
		m_UIScriptViewMenu.SetSubMenu(m_ChildMenu);
	}
	
	bool UseMouse()
	{
		return false;
	}
	
	//! see <exclude> in bin/specific.xml
	array<string> GetInputExcludes()
	{
		return {};
	}
	
	//! list of UA inputs to force
	array<int> GetInputRestrictions()
	{
		return {};
	}
	
	void Close()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(_Close);
	}
	
	private void _Close()
	{
		delete this;
	}
	
	UIScriptViewMenu GetUIScriptViewMenu()
	{
		return m_UIScriptViewMenu;
	}
	
	UIScriptedMenu GetChildMenu()
	{
		return m_ChildMenu;
	}
}