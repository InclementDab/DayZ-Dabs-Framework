modded class LayoutHolder
{
    void LayoutHolder(LayoutHolder parent)
	{
        // Loads widget_names into variables, if applicable
		if (m_MainWidget && UsesWidgetVariables()) {
			ScriptView.LoadWidgetsAsVariables(this, m_MainWidget);
		}
	}

	bool UsesWidgetVariables()
	{
		return false;
	}
}