class OptionMenuCustom: Managed
{	
	protected ref OptionsTabView m_OptionsTabView;
	
	void OptionMenuCustom(Widget parent, notnull ProfileSettings settings, notnull ScriptCaller on_changed)
	{
		m_OptionsTabView = new OptionsTabView(settings, on_changed);
		parent.AddChild(m_OptionsTabView.GetLayoutRoot());
	}
		
	bool IsChanged()
	{
		return m_OptionsTabView.IsChanged();
	}
		
	bool NeedsRestart()
	{
		return m_OptionsTabView.NeedsRestart();
	}
	
	void Apply()
	{
		m_OptionsTabView.Apply();
	}
	
	void Revert()
	{
		m_OptionsTabView.Revert();
	}
}