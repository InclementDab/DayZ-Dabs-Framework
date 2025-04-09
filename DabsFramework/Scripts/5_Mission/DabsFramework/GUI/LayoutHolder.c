modded class LayoutHolder
{
    void LayoutHolder(LayoutHolder parent)
	{
        // Loads widget_names into variables, if applicable
		if (m_MainWidget && UsesWidgetVariables()) {
			auto property_map = new PropertyTypeHashMap(Type());
			if (property_map) {
				ScriptView.LoadWidgetsAsVariables(this, property_map, m_MainWidget);
			}
		}
	}

	bool UsesWidgetVariables()
	{
		return false;
	}
}