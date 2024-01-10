class OptionsCategoryView: ScriptView
{
	protected OptionsCategoryViewController m_TemplateController;
	OptionsCategoryViewController GetTemplateController()
	{
		return m_TemplateController;
	}
	
	protected ProfileSettings m_ProfileSettings;
	protected ProfileSettingsCategory m_ProfileSettingsCategory;
	
	void OptionsCategoryView(notnull ProfileSettings profile_settings, notnull ProfileSettingsCategory settings_category, notnull ScriptCaller on_changed)
	{
		m_TemplateController = OptionsCategoryViewController.Cast(m_Controller);
		
		m_ProfileSettings = profile_settings;
		m_ProfileSettingsCategory = settings_category;
		
		m_TemplateController.HeaderName = m_ProfileSettingsCategory.Header;
		m_TemplateController.NotifyPropertyChanged("HeaderName");
		
		foreach (ProfileSettingEntry settings_entry: m_ProfileSettingsCategory.Settings) {
			m_TemplateController.Options.Insert(new OptionView(m_ProfileSettings, settings_entry, on_changed));
		}
	}
	
	bool IsChanged()
	{
		for (int i = 0; i < m_TemplateController.Options.Count(); i++) {
			OptionView child_view = m_TemplateController.Options[i];
			if (child_view && child_view.IsChanged()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool NeedsRestart()
	{
		for (int i = 0; i < m_TemplateController.Options.Count(); i++) {
			OptionView child_view = m_TemplateController.Options[i];
			if (child_view && child_view.NeedsRestart()) {
				return true;
			}
		}
		
		return false;
	}
	
	void Apply()
	{
		for (int i = 0; i < m_TemplateController.Options.Count(); i++) {
			OptionView child_view = m_TemplateController.Options[i];
			if (child_view) {
				child_view.Apply();
			}
		}
	}
	
	void Revert()
	{
		for (int i = 0; i < m_TemplateController.Options.Count(); i++) {
			OptionView child_view = m_TemplateController.Options[i];
			if (child_view) {
				child_view.Revert();
			}
		}
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\options_category.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionsCategoryViewController;
	}
}