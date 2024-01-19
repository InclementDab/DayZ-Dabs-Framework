class OptionView: ScriptView
{
	protected OptionViewController m_TemplateController;
	OptionViewController GetTemplateController()
	{
		return m_TemplateController;
	}
	
	protected ProfileSettings m_ProfileSettings;
	protected ProfileSettingEntry m_ProfileSettingEntry;
	
	TextWidget Text;
		
	void OptionView(notnull ProfileSettings profile_settings, notnull ProfileSettingEntry setting_entry, notnull ScriptCaller on_changed)
	{
		m_TemplateController = OptionViewController.Cast(m_Controller);
		
		m_ProfileSettingEntry = setting_entry;
		m_ProfileSettings = profile_settings;
		
		m_TemplateController.Label = m_ProfileSettingEntry.GetDisplayName();
		m_TemplateController.NotifyPropertyChanged("Label");
		
		m_TemplateController.Option.Insert(m_ProfileSettingEntry.CreateMenuOption(m_ProfileSettings, on_changed));
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		for (int i = 0; i < m_TemplateController.Option.Count(); i++) {
			if (m_TemplateController.Option[i].Text) {
				m_TemplateController.Option[i].Text.SetColor(ARGB(255, 200, 0, 0));
			}
		}
		
		Text.SetColor(ARGB(255, 200, 0, 0));
		m_LayoutRoot.SetColor(ARGB(255, 0, 0, 0));
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{	
		for (int i = 0; i < m_TemplateController.Option.Count(); i++) {
			if (m_TemplateController.Option[i].Text) {
				m_TemplateController.Option[i].Text.SetColor(ARGB(255, 255, 255, 255));
			}
		}
		
		Text.SetColor(ARGB(255, 255, 255, 255));
		m_LayoutRoot.SetColor(ARGB(0, 255, 255, 255));
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	bool IsChanged()
	{
		for (int i = 0; i < m_TemplateController.Option.Count(); i++) {
			OptionSelectorViewBase child_view = m_TemplateController.Option[i];
			if (child_view && child_view.IsChanged()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool NeedsRestart()
	{
		for (int i = 0; i < m_TemplateController.Option.Count(); i++) {
			OptionSelectorViewBase child_view = m_TemplateController.Option[i];
			if (child_view && child_view.NeedsRestart()) {
				return true;
			}
		}
		
		return false;
	}
	
	void Apply()
	{
		for (int i = 0; i < m_TemplateController.Option.Count(); i++) {
			OptionSelectorViewBase child_view = m_TemplateController.Option[i];
			if (child_view) {
				child_view.Apply();
			}
		}
	}
	
	void Revert()
	{
		for (int i = 0; i < m_TemplateController.Option.Count(); i++) {
			OptionSelectorViewBase child_view = m_TemplateController.Option[i];
			if (child_view) {
				child_view.Revert();
			}
		}
	}
	
	ProfileSettingEntry GetSettingsEntry()
	{
		return m_ProfileSettingEntry;
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\options_item.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionViewController;
	}
}