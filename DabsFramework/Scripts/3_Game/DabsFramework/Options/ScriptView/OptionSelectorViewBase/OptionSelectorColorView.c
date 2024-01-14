class OptionSelectorColorView: OptionSelectorViewBase
{	
	SliderWidget RedSlider, GreenSlider, BlueSlider, AlphaSlider;
	EditBoxWidget RedEditBox, GreenEditBox, BlueEditBox, AlphaEditBox;
	TextWidget RedLabel, GreenLabel, BlueLabel, AlphaLabel;
	
	Widget ColorPickerWrapper;
	Widget AlphaSliderRoot;
	
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
		m_OptionSelectorColorViewController.NotifyPropertiesChanged({"Value", "Red", "Green", "Blue", "Hue", "Saturation", "Var"});

		m_OptionSelectorColorViewController.StartValue = m_OptionSelectorColorViewController.Value;
		
		if (!profile_setting_color.GetAllowAlpha()) {
			AlphaSliderRoot.Show(false);
		}
	}
		
	void OnToggleExecute(ButtonCommandArgs args)
	{	
		ColorPickerWrapper.Show(!ColorPickerWrapper.IsVisible());
		
	}
	
	void OnSelectExecute(ButtonCommandArgs args)
	{
		
	}
	
	void OnCancelExecute(ButtonCommandArgs args)
	{
		m_OptionSelectorColorViewController.StartValue = m_OptionSelectorColorViewController.Value;
		m_OptionSelectorColorViewController.NotifyPropertyChanged("Value");
	}
		
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		switch (w) {
			case RedSlider: {
				RedEditBox.Show(true);
				RedLabel.Show(false);
				break;
			}			
			
			case GreenSlider: {
				GreenEditBox.Show(true);
				GreenLabel.Show(false);
				break;
			}			
			
			case BlueSlider: {
				BlueEditBox.Show(true);
				BlueLabel.Show(false);
				break;
			}			
			
			case AlphaSlider: {
				AlphaEditBox.Show(true);
				AlphaEditBox.Show(false);
				break;
			}
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (finished) {
			int determined_value = ARGB(m_OptionSelectorColorViewController.AlphaUserInput.ToInt(), m_OptionSelectorColorViewController.RedUserInput.ToInt(), m_OptionSelectorColorViewController.GreenUserInput.ToInt(), m_OptionSelectorColorViewController.BlueUserInput.ToInt());
			switch (w) {
				case RedEditBox:
				case GreenEditBox:
				case BlueEditBox:
				case AlphaEditBox: {
					m_OptionSelectorColorViewController.Value = determined_value;
					m_OptionSelectorColorViewController.NotifyPropertyChanged("Value");					
					break;
				}
			}
			
			switch (w) {
				case RedEditBox: {
					RedEditBox.Show(false);
					RedLabel.Show(true);
					return true;
				}			
				
				case GreenEditBox: {
					GreenEditBox.Show(false);
					GreenLabel.Show(true);
					return true;
				}			
				
				case BlueEditBox: {
					BlueEditBox.Show(false);
					BlueLabel.Show(true);
					return true;
				}			
				
				case AlphaEditBox: {
					AlphaEditBox.Show(false);
					AlphaEditBox.Show(true);
					return true;
				}
			}
		}
		
		return super.OnChange(w, x, y, finished);
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
		m_OptionSelectorColorViewController.Value = m_OptionSelectorColorViewController.StartValue;
		m_OptionSelectorColorViewController.NotifyPropertyChanged("Value");
	}
	
	override bool IsChanged()
	{
		return (m_OptionSelectorColorViewController.Value != m_OptionSelectorColorViewController.StartValue);
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