modded class LayoutHolder
{
    void LayoutHolder(LayoutHolder parent)
	{
        // Loads widget_names into variables, if applicable
		if (m_MainWidget) {
			ScriptView.LoadWidgetsAsVariables(this, new PropertyTypeHashMap(Type()), m_MainWidget);
		}
	}
}