// Wrapper for UIScriptedMenu in ScriptView
// mainly used by ScriptView::GetScriptedMenu
class ScriptViewMenu: UIScriptedMenu
{
	protected ScriptView m_ScriptView;
	
	//! When the UI manager enters a menu, it changes the handler over to this entity
	//		manually calling it will propagate it appropriately
	protected ScriptedViewBaseHandler m_ScriptedViewBaseHandler;

	void ScriptViewMenu(ScriptView script_view)
	{
		m_ScriptView = script_view;
		m_ScriptedViewBaseHandler = m_ScriptView.GetController().GetHandler();
		if (!m_ScriptedViewBaseHandler) {
			Error("Could not find handler from function");
		}
		
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

	override bool OnClick(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBaseHandler.OnClick(w, x, y, button);
	}

	override bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		return m_ScriptedViewBaseHandler.OnModalResult(w, x, y, code, result);
	}

	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBaseHandler.OnDoubleClick(w, x, y, button);
	}

	override bool OnSelect(Widget w, int x, int y)
	{
		return m_ScriptedViewBaseHandler.OnSelect(w, x, y);
	}

	override bool OnItemSelected(Widget w, int x, int y, int row, int column, int oldRow, int oldColumn)
	{
		return m_ScriptedViewBaseHandler.OnItemSelected(w, x, y, row, column, oldRow, oldColumn);
	}

	override bool OnFocus(Widget w, int x, int y)
	{
		return m_ScriptedViewBaseHandler.OnFocus(w, x, y);
	}

	override bool OnFocusLost(Widget w, int x, int y)
	{
		return m_ScriptedViewBaseHandler.OnFocusLost(w, x, y);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		return m_ScriptedViewBaseHandler.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		return m_ScriptedViewBaseHandler.OnMouseLeave(w, enterW, x, y);
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		return m_ScriptedViewBaseHandler.OnMouseWheel(w, x, y, wheel);
	}

	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBaseHandler.OnMouseButtonDown(w, x, y, button);
	}

	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		return m_ScriptedViewBaseHandler.OnMouseButtonUp(w, x, y, button);
	}

	override bool OnController(Widget w, int control, int value)
	{
		return m_ScriptedViewBaseHandler.OnController(w, control, value);
	}

	override bool OnKeyDown(Widget w, int x, int y, int key)
	{
		return m_ScriptedViewBaseHandler.OnKeyDown(w, x, y, key);
	}

	override bool OnKeyUp(Widget w, int x, int y, int key)
	{
		return m_ScriptedViewBaseHandler.OnKeyUp(w, x, y, key);
	}

	override bool OnKeyPress(Widget w, int x, int y, int key)
	{
		return m_ScriptedViewBaseHandler.OnKeyPress(w, x, y, key);
	}

	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		return m_ScriptedViewBaseHandler.OnChange(w, x, y, finished);
	}

	override bool OnDrag(Widget w, int x, int y)
	{
		return m_ScriptedViewBaseHandler.OnDrag(w, x, y);
	}

	override bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBaseHandler.OnDragging(w, x, y, reciever);
	}

	override bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBaseHandler.OnDraggingOver(w, x, y, reciever);
	}

	override bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBaseHandler.OnDrop(w, x, y, reciever);
	}

	override bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		return m_ScriptedViewBaseHandler.OnDropReceived(w, x, y, reciever);
	}
}