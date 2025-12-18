// Essentially the same as ScriptView except GetType is now a Template
class ScriptViewTemplate<Class T> : ScriptView
{
	protected T m_TemplateController;

	T GetTemplateController()
	{
		return m_TemplateController;
	}

	void ScriptViewTemplate()
	{
		if (!Class.CastTo(m_TemplateController, m_Controller)) {
			typename t = T;
			Error("Error in casting of Template Controller %1", t.ToString());
			return;
		}
	}

	private	override typename GetControllerType()
	{
		return T;
	}
}