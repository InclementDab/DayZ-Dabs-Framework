// Wrapper for UIScriptedMenu in ScriptView
// mainly used by ScriptView::GetScriptedMenu
class ScriptViewMenu: UIScriptedMenu
{
	protected ScriptView m_ScriptView;

	void ScriptViewMenu(ScriptView script_view)
	{
		m_ScriptView = script_view;
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