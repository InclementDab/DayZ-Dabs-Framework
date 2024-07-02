modded class LayoutHolder
{
    void LayoutHolder(LayoutHolder parent)
	{
        // Loads widget_names into variables, if applicable
		if (m_MainWidget && UsesWidgetVariables()) {
			ScriptView.LoadWidgetsAsVariables(this, new PropertyTypeHashMap(Type()), m_MainWidget);
		}
	}

	bool UsesWidgetVariables()
	{
		return false;
	}
}