class SymbolHandler: ScriptedWidgetEventHandler
{
	reference string Symbol_Name;
	
	reference int Symbol_Size = 2;
	
	void OnWidgetScriptInit(Widget w)
	{
		Symbols symbol = string.Format("image:%1", Symbol_Name);
		
		ImageWidget image_widget = ImageWidget.Cast(w);
		if (!image_widget) {
			ErrorEx("SymbolHandler must be used on image widget");
			return;
		}
		
		symbol.Load(image_widget, Symbol_Size);
	}
}
