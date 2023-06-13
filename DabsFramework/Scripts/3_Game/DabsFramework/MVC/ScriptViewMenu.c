//@ It is your responsibility to manage the memory for this menu object
class ScriptViewMenu: ScriptView
{		
	protected ref UIScriptViewMenu m_UIScriptViewMenu = new UIScriptViewMenu(this);
	
	//@ This menu will control its subsequent children UI menus, but NOT children ScriptViewMenu!!!
	protected ref UIScriptedMenu m_ChildMenu;
	
	void ScriptViewMenu()
	{
		// We are the parent menu
		g_Game.GetUIManager().ShowScriptedMenu(m_UIScriptViewMenu, g_Game.GetUIManager().GetMenu());
		
		// Handle input excludes
		g_Game.GetMission().AddActiveInputExcludes(GetInputExcludes());		
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
	
	void ~ScriptViewMenu()
	{
		// This lazy calls the destructor
		if (m_UIScriptViewMenu) {
			g_Game.GetUIManager().HideScriptedMenu(m_UIScriptViewMenu);
		}
		
		// input excludes
		if (g_Game.GetMission()) {
			g_Game.GetMission().RemoveActiveInputExcludes(GetInputExcludes());
		}
		
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
	
	void ShowDialog(string caption, string text, int id, int buttons /*DBT_*/, int default_button /*DBB_*/, int type /*DMT_*/)
	{
		g_Game.GetUIManager().ShowDialog(caption, text, id, buttons, default_button, type, m_UIScriptViewMenu);
	}
	
	//@ important functions called by the UI manager when entering and leaving child menus
	//		as much as i want to put this in the constructor, you cant because menus can be hidden and shown as they juggle
	//		back and fourth. and we dont want to delete a menu when showing a child menu
	void OnMenuEnter()
	{
		if (UseMouse()) {
			g_Game.GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
			g_Game.GetUIManager().ShowUICursor(true);
		}
	}
	
	void OnMenuExit()
	{		
		// Mouse control
		if (UseMouse()) {
			g_Game.GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
		}

		g_Game.GetUIManager().ShowUICursor(m_UIScriptViewMenu.GetParentMenu() && m_UIScriptViewMenu.GetParentMenu().UseMouse());
	}
	
	void EnterChildMenu(int id)
	{
		EnterChildMenu(GetGame().GetMission().CreateScriptedMenu(id));
	}
		
	void EnterChildMenu(ScriptViewMenu menu)
	{
		EnterChildMenu(menu.GetUIScriptViewMenu());
	}
	
	void EnterChildMenu(UIScriptedMenu scripted_menu)
	{
		m_ChildMenu = scripted_menu;
		g_Game.GetUIManager().ShowScriptedMenu(m_ChildMenu, m_UIScriptViewMenu);
	}
	
	//! Is mouse cursor shown on menu enter
	bool UseMouse()
	{
		return true;
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