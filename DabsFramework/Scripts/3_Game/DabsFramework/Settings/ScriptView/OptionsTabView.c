class OptionsTabView: ScriptView
{
	protected OptionsTabViewController m_TemplateController;
	OptionsTabViewController GetTemplateController()
	{
		return m_TemplateController;
	}
	
	protected ProfileSettings m_ProfileSettings;
	
	void OptionsTabView(notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		m_TemplateController = OptionsTabViewController.Cast(m_Controller);
		m_ProfileSettings = settings;
		
		array<ref ProfileSettingsCategory> categories = m_ProfileSettings.GetCategories();		
		foreach (ProfileSettingsCategory category: categories) {
			m_TemplateController.OptionCategories.Insert(new OptionsCategoryView(m_ProfileSettings, category, on_changed));
		}
	}
	
	bool IsChanged()
	{
		for (int i = 0; i < m_TemplateController.OptionCategories.Count(); i++) {
			OptionsCategoryView child_view = m_TemplateController.OptionCategories[i];
			if (child_view && child_view.IsChanged()) {
				return true;
			}
		}
		
		return false;
	}
	
	bool NeedsRestart()
	{
		for (int i = 0; i < m_TemplateController.OptionCategories.Count(); i++) {
			OptionsCategoryView child_view = m_TemplateController.OptionCategories[i];
			if (child_view && child_view.NeedsRestart()) {
				return true;
			}
		}
		
		return false;
	}
	
	void Apply()
	{
		for (int i = 0; i < m_TemplateController.OptionCategories.Count(); i++) {
			OptionsCategoryView child_view = m_TemplateController.OptionCategories[i];
			if (child_view) {
				child_view.Apply();
			}
		}
	}
	
	void Revert()
	{
		for (int i = 0; i < m_TemplateController.OptionCategories.Count(); i++) {
			OptionsCategoryView child_view = m_TemplateController.OptionCategories[i];
			if (child_view) {
				child_view.Revert();
			}
		}
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\options_tab.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionsTabViewController;
	}
}