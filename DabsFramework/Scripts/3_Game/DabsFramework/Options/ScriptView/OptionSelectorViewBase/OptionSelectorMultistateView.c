class OptionSelectorMultistateView: OptionSelectorViewBase
{
	protected string m_CurrentOption;
	protected ProfileSettingMultistate m_ProfileSettingMultistate;
			
	void OptionSelectorMultistateView(notnull ProfileSettings profile_settings, notnull ScriptCaller on_changed, notnull ProfileSettingMultistate profile_setting_multistate)
	{
		m_ProfileSettingMultistate = profile_setting_multistate;
		
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingMultistate.GetVariableName()]);
		if (!type_converter) {
			Error(string.Format("Invalid variable type on registry type=%1", properties[m_ProfileSettingMultistate.GetVariableName()]));
			return;
		}
		
		type_converter.GetFromController(m_ProfileSettings, m_ProfileSettingMultistate.GetVariableName(), 0);
		array<string> options_list = m_ProfileSettingMultistate.GetPossibleStates();
		m_CurrentOption = options_list[type_converter.GetInt()];
		Text.SetText(m_CurrentOption);
	}
	
	void OnNextExecute(ButtonCommandArgs args)
	{
		array<string> options_list = m_ProfileSettingMultistate.GetPossibleStates();
		int current = options_list.Find(m_CurrentOption);
		m_CurrentOption = options_list[Math.Rollover(current + 1, 1, options_list.Count())];
		Text.SetText(m_CurrentOption);
		
		m_OnChanged.Invoke();
	}
	
	void OnPreviousExecute(ButtonCommandArgs args)
	{
		array<string> options_list = m_ProfileSettingMultistate.GetPossibleStates();
		int current = options_list.Find(m_CurrentOption);
		m_CurrentOption = options_list[Math.Rollover(current - 1, 1, options_list.Count())];
		Text.SetText(m_CurrentOption);
		
		m_OnChanged.Invoke();
	}
	
	string GetCurrentOption()
	{
		return m_CurrentOption;
	}
	
	override void Apply()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingMultistate.GetVariableName()]);
		type_converter.SetInt(m_ProfileSettingMultistate.GetPossibleStates().Find(m_CurrentOption));
		type_converter.SetToController(m_ProfileSettings, m_ProfileSettingMultistate.GetVariableName(), 0);
		m_ProfileSettings.Save();
	}
	
	override void Revert()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingMultistate.GetVariableName()]);
		type_converter.GetFromController(m_ProfileSettings, m_ProfileSettingMultistate.GetVariableName(), 0);
		array<string> options_list = m_ProfileSettingMultistate.GetPossibleStates();
		m_CurrentOption = options_list[type_converter.GetInt()];
		Text.SetText(m_CurrentOption);
	}
	
	override bool IsChanged()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingMultistate.GetVariableName()]);
		type_converter.GetFromController(m_ProfileSettings, m_ProfileSettingMultistate.GetVariableName(), 0);
		
		array<string> options_list = m_ProfileSettingMultistate.GetPossibleStates();
		return (m_CurrentOption != options_list[type_converter.GetInt()]);
	}
	
	override bool NeedsRestart()
	{
		return (IsChanged() && m_ProfileSettingMultistate.GetRequiresRestart());
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\option_types\\option_multistate.layout";
	}
}