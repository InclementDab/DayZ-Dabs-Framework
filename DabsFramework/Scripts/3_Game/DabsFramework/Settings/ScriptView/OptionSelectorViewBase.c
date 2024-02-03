class OptionSelectorViewBase: ScriptView
{
	protected ProfileSettings m_ProfileSettings;
	protected ref ScriptCaller m_OnChanged;
	
	TextWidget Text;
	
	void OptionSelectorViewBase(notnull ProfileSettings profile_settings, notnull ScriptCaller on_changed)
	{
		m_ProfileSettings = profile_settings;
		m_OnChanged = on_changed;
	}
	
	void Apply()
	{
	}
	
	void Revert()
	{
	}
	
	bool IsChanged()
	{
		return false;
	}
	
	bool NeedsRestart()
	{
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		Widget image = w.FindAnyWidget("Image");
		
		if (image) {
			image.SetColor(ARGB(255, 200, 0, 0));
		}
		
		if (Text) {
			Text.SetColor(ARGB(255, 200, 0, 0));
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{	
		Widget image = w.FindAnyWidget("Image");	
		if (image) {
			image.SetColor(ARGB(255, 255, 255, 255));
		}
		
		if (Text) {
			Text.SetColor(ARGB(255, 255, 255, 255));
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}