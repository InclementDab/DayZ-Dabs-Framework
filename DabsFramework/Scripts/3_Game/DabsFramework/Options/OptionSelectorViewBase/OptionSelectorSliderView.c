class OptionSelectorSliderViewController: ViewController
{
	// could be unique for each setting
	static const int DECIMAL_PRECISION = 3;
	
	ScriptCaller OnValueChanged;
	
	float Min, Max, Value; // Value is 0..1
	
	float ValueActual; // real value to be set in settings / config
	
	string ValueUserInput; // value for user to interface with and attempt to input
	
	void OnUserInputFinished()
	{
		ValueActual = ValueUserInput.ToFloat();
		NotifyPropertyChanged("ValueActual");
	}
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
			
			case "Value": {
				// Update settings value
				ValueActual = Math.Lerp(Min, Max, Value);
				NotifyPropertyChanged("ValueActual", false);
				
				// Update user input visualization
				ValueUserInput = ValueActual.ToString();
				int decimal_idx = ValueUserInput.IndexOf(".");
				if (decimal_idx != -1 && ValueUserInput.Length() - decimal_idx > DECIMAL_PRECISION) {					
					ValueUserInput = ValueUserInput.SubstringInverted(ValueUserInput, decimal_idx + DECIMAL_PRECISION + 1, ValueUserInput.Length());
				}
				
				NotifyPropertyChanged("ValueUserInput", false);
				
				if (OnValueChanged) {
					OnValueChanged.Invoke();
				}
				
				break;
			}
			
			case "ValueActual": {
				ValueActual = Math.Clamp(ValueActual, Min, Max);
				Value = Math.InverseLerp(Min, Max, ValueActual);
				NotifyPropertyChanged("Value");
				break;
			}
					
			case "ValueUserInput": {
				for (int i = 0; i < ValueUserInput.Length(); i++) {
					if (!StringEvaluaterEvaluater.IsNumeric(ValueUserInput[i])) {
						ValueUserInput.Set(i, string.Empty); // enfusions broken this morning
						NotifyPropertyChanged("ValueUserInput", false);
					}
				}
				
				break;
			}
		}
	}
}

class OptionSelectorSliderView: OptionSelectorViewBase
{
	protected OptionSelectorSliderViewController m_OptionSelectorSliderViewController;
	protected ProfileSettingSlider m_ProfileSettingSlider;
	
	protected float m_StartingValue;
	
	SliderWidget Slider;
	TextWidget Label;
	EditBoxWidget EditBox;
	
	void OptionSelectorSliderView(notnull ProfileSettings profile_settings, notnull ScriptCaller on_changed, notnull ProfileSettingSlider profile_setting_slider)
	{
		m_ProfileSettingSlider = profile_setting_slider;
		
		m_OptionSelectorSliderViewController = OptionSelectorSliderViewController.Cast(m_Controller);
		m_OptionSelectorSliderViewController.OnValueChanged = on_changed;
		
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingSlider.GetVariableName()]);
		if (!type_converter) {
			Error(string.Format("Invalid variable type on registry type=%1", properties[m_ProfileSettingSlider.GetVariableName()]));
			return;
		}
		
		type_converter.GetFromController(m_ProfileSettings, m_ProfileSettingSlider.GetVariableName(), 0);
		
		// progress bar will need to be 01 always since you cant set min / max dynamic
		m_OptionSelectorSliderViewController.Min = m_ProfileSettingSlider.GetMin();
		m_OptionSelectorSliderViewController.Max = m_ProfileSettingSlider.GetMax();
		m_OptionSelectorSliderViewController.ValueActual = type_converter.GetFloat();
		m_OptionSelectorSliderViewController.NotifyPropertyChanged("ValueActual");
		
		m_StartingValue = m_OptionSelectorSliderViewController.ValueActual;
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
		
		if (GetFocus() == EditBox) {
			Slider.SetColor(ARGB(255, 200, 0, 0));
		}
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		if (w == EditBox && finished) {
			m_OptionSelectorSliderViewController.OnUserInputFinished();
			Label.Show(true);
			EditBox.Show(false);
			return true;
		}
		
		return super.OnChange(w, x, y, finished);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		return super.OnClick(w, x, y, button);
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (w == Slider) {
			EditBox.Show(true);
			Label.Show(false);
			return true;
		}
		
		return super.OnDoubleClick(w, x, y, button);
	}
		
	override bool OnMouseEnter(Widget w, int x, int y)
	{		
		switch (w) {
			case Label:
			case Slider: {
				Slider.SetColor(ARGB(255, 200, 0, 0));
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ((w == Slider && enterW != Label) || (w == Label && enterW != Slider)) {
			Slider.SetColor(ARGB(140, 255, 255, 255));
		}
			
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	override void Apply()
	{
		PropertyTypeHashMap properties = new PropertyTypeHashMap(m_ProfileSettings.Type());
		TypeConverter type_converter = MVC.GetTypeConversion(properties[m_ProfileSettingSlider.GetVariableName()]);
		type_converter.SetFloat(m_OptionSelectorSliderViewController.ValueActual);
		type_converter.SetToController(m_ProfileSettings, m_ProfileSettingSlider.GetVariableName(), 0);
		m_ProfileSettings.Save();
		m_StartingValue = m_OptionSelectorSliderViewController.ValueActual;
	}
	
	override void Revert()
	{
		m_OptionSelectorSliderViewController.ValueActual = m_StartingValue;
		m_OptionSelectorSliderViewController.NotifyPropertyChanged("ValueActual");
	}
	
	override bool IsChanged()
	{
		return (m_StartingValue != m_OptionSelectorSliderViewController.ValueActual);
	}
	
	override bool NeedsRestart()
	{
		return (IsChanged() && m_ProfileSettingSlider.GetRequiresRestart());
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\option_types\\option_slider.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionSelectorSliderViewController;
	}
}