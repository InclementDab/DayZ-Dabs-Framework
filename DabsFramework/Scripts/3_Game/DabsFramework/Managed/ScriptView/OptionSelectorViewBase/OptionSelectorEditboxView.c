class OptionSelectorEditboxView: OptionSelectorViewBase
{
	protected OptionSelectorEditboxViewController m_OptionSelectorEditboxViewController;
	protected ProfileSettingText m_ProfileSettingText;
	
	protected string m_StartValue;
	
	void OptionSelectorEditboxView(notnull ProfileSettings profile_settings, notnull ScriptCaller on_changed, notnull ProfileSettingText profile_setting_text)
	{
		m_ProfileSettingText = profile_setting_text;
		m_OptionSelectorEditboxViewController = OptionSelectorEditboxViewController.Cast(m_Controller);
		m_OptionSelectorEditboxViewController.OnValueChanged = on_changed;
		
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = GetDayZGame().GetTypeConversion(properties[m_ProfileSettingText.GetVariableName()]);
		if (!type_converter) {
			Error(string.Format("Invalid variable type on registry type=%1", properties[m_ProfileSettingText.GetVariableName()]));
			return;
		}
		
		type_converter.GetFromController(m_ProfileSettings, m_ProfileSettingText.GetVariableName(), 0);
		
		m_OptionSelectorEditboxViewController.Value = type_converter.GetString();
		m_OptionSelectorEditboxViewController.NotifyPropertyChanged("Value");
		
		m_StartValue = m_OptionSelectorEditboxViewController.Value;
	}
	
	override void Apply()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = GetDayZGame().GetTypeConversion(properties[m_ProfileSettingText.GetVariableName()]);
		type_converter.SetString(m_OptionSelectorEditboxViewController.Value);
		type_converter.SetToController(m_ProfileSettings, m_ProfileSettingText.GetVariableName(), 0);
		m_ProfileSettings.Save();
	}
	
	override void Revert()
	{
		m_OptionSelectorEditboxViewController.Value = m_StartValue;
		m_OptionSelectorEditboxViewController.NotifyPropertyChanged("Value");
	}
	
	override bool IsChanged()
	{
		return (m_OptionSelectorEditboxViewController.Value != m_StartValue);
	}
	
	override bool NeedsRestart()
	{
		return (IsChanged() && m_ProfileSettingText.GetRequiresRestart());
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\option_types\\option_editbox.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionSelectorEditboxViewController;
	}
}