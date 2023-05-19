class ScriptViewMenuTemplate<Class T>: ScriptViewMenu
{
	protected T m_TemplateController;

	T GetTemplateController()
	{
		return m_TemplateController;
	}

	void ScriptViewMenuTemplate()
	{
		if (!Class.CastTo(m_TemplateController, m_Controller)) {
			Error("Error in casting of Template Controller %1", T.ToString());
			return;
		}
	}

	private	override typename GetControllerType()
	{
		return T;
	}
}