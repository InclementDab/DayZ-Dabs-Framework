// Wrapper for UIScriptedMenu in ScriptView
// mainly used by ScriptView::GetScriptedMenu
class ScriptViewMenu: UIScriptedMenu
{
	protected ScriptView m_ScriptView;

	void ScriptViewMenu(ScriptView script_view)
	{
		m_ScriptView = script_view;
				
		SetID(MENU_ANY);
	}
		
	// called by engine when ui manager hides / shows
	override void OnShow()
	{	
		LockControls();
		if (m_ScriptView) {
			m_ScriptView.OnScriptViewMenuEnter(this);
		}
	}

	override void OnHide()
	{
		UnlockControls();
		if (m_ScriptView) {
			m_ScriptView.OnScriptViewMenuExit(this);
		}
	}
	
	override bool UseMouse()
	{
		return (m_ScriptView && m_ScriptView.UseMouse());
	}
	
	override bool UseKeyboard()
	{
		return (m_ScriptView && m_ScriptView.UseKeyboard());
	}
	
	override bool UseGamepad()
	{
		return (m_ScriptView && m_ScriptView.UseGamepad());
	}

	override Widget Init()
	{
		layoutRoot = m_ScriptView.GetLayoutRoot();
		return layoutRoot;
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
    {
        return m_ScriptView.OnModalResult(w, x, y, code, result);
    }
}