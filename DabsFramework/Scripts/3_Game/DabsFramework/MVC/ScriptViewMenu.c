// Wrapper for UIScriptedMenu in ScriptView
// mainly used by ScriptView::GetScriptedMenu
class ScriptViewMenu: UIScriptedMenu
{
	protected ScriptView m_ScriptView;

	void ScriptViewMenu(ScriptView script_view)
	{
		m_ScriptView = script_view;
		
		if (m_ScriptView.UseUIManager()) {
			GetGame().GetUIManager().ShowScriptedMenu(this, null); // null, todo?
		}
	}
		
	// called by engine when ui manager hides / shows
	override void OnShow()
	{
		if (m_ScriptView) {
			m_ScriptView.GetLayoutRoot().Show(true);
			m_ScriptView.OnScriptViewMenuEnter(this);
		}
	}

	override void OnHide()
	{
		if (m_ScriptView) {
			m_ScriptView.GetLayoutRoot().Show(false);
			m_ScriptView.OnScriptViewMenuExit(this);
		}
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