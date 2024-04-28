class SymbolHandler: ScriptedWidgetEventHandler
{
	reference string Symbol_Name;
	
	reference int Symbol_Size = 2;
	
	protected ImageWidget m_ImageWidget;
	
	void OnWidgetScriptInit(Widget w)
	{
		Symbols symbol = string.Format("image:%1", Symbol_Name);
		
		m_ImageWidget = ImageWidget.Cast(w);
		if (!m_ImageWidget) {
			ErrorEx("SymbolHandler must be used on image widget");
			return;
		}
		
		symbol.Load(m_ImageWidget, Symbol_Size);
	}
	
	void SetSize(int symbol_size)
	{
		Symbols symbol = string.Format("image:%1", Symbol_Name);
		if (m_ImageWidget) {
			symbol.Load(m_ImageWidget, symbol_size);
		}
	}
}
