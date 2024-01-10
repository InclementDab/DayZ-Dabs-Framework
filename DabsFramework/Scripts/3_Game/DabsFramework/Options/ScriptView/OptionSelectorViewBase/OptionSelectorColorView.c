class OptionSelectorColorViewController: ViewController
{
	int Value;
	
	ScriptCaller OnValueChanged;
}

class OptionSelectorColorView: OptionSelectorViewBase
{
	protected OptionSelectorColorViewController m_OptionSelectorColorViewController;
	protected ProfileSettingColor m_ProfileSettingsColor;
	
	protected int m_StartValue;
	
	void OptionSelectorColorView(notnull ProfileSettings profile_settings, notnull ScriptCaller on_changed, notnull ProfileSettingColor profile_setting_color)
	{
		m_ProfileSettingsColor = profile_setting_color;
		
		m_OptionSelectorColorViewController = OptionSelectorColorViewController.Cast(m_Controller);
		m_OptionSelectorColorViewController.OnValueChanged = on_changed;
		
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingsColor.GetVariableName()]);
		if (!type_converter) {
			Error(string.Format("Invalid variable type on registry type=%1", properties[m_ProfileSettingsColor.GetVariableName()]));
			return;
		}
		
		type_converter.GetFromController(m_ProfileSettings, m_ProfileSettingsColor.GetVariableName(), 0);
		
		m_OptionSelectorColorViewController.Value = type_converter.GetInt();
		m_OptionSelectorColorViewController.NotifyPropertyChanged("Value");
		
		m_StartValue = m_OptionSelectorColorViewController.Value;
	}
	
	override void Apply()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingsColor.GetVariableName()]);
		type_converter.SetInt(m_OptionSelectorColorViewController.Value);
		type_converter.SetToController(m_ProfileSettings, m_ProfileSettingsColor.GetVariableName(), 0);
		m_ProfileSettings.Save();
	}
	
	override void Revert()
	{
		m_OptionSelectorColorViewController.Value = m_StartValue;
		m_OptionSelectorColorViewController.NotifyPropertyChanged("Value");
	}
	
	override bool IsChanged()
	{
		return (m_OptionSelectorColorViewController.Value != m_StartValue);
	}
	
	override bool NeedsRestart()
	{
		return (IsChanged() && m_ProfileSettingsColor.GetRequiresRestart());
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\option_types\\option_color.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionSelectorColorViewController;
	}
}