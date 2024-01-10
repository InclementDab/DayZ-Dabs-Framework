class OptionSelectorColorViewController: ViewController
{
	static const int COLOR_GRADIENT_ACCURACY = 3;
	
	int Value;
	
	int Alpha = 255, Red, Green, Blue;
	float Hue, Saturation, Var;
	
	ScriptCaller OnValueChanged;
	
	CanvasWidget ColorPicker;
	CanvasWidget ColorGradient;
	CanvasWidget ColorLightnesss;
	
	void OptionSelectorColorViewController()
	{

	}
	
	override void OnWidgetScriptInit(Widget w)
	{
		super.OnWidgetScriptInit(w);
		
		float size_x, size_y;
		ColorPicker.GetScreenSize(size_x, size_y);
				
		float hsv_size_x, hsv_size_y;
		ColorGradient.GetScreenSize(hsv_size_x, hsv_size_y);
		
		float hsl_size_x, hsl_size_y;
		ColorLightnesss.GetScreenSize(hsl_size_x, hsl_size_y);
		
		ColorPicker.Clear();
		ColorGradient.Clear();
		ColorLightnesss.Clear();
		
		for (int i = 0; i <= size_y; ) {
			float y_value = i / size_y;
			
			for (int j = 0; j <= size_x; ) {
				float x_value = j / size_x;	
				ColorPicker.DrawLine(i, j, i + COLOR_GRADIENT_ACCURACY, j + COLOR_GRADIENT_ACCURACY, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Hue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), Alpha));
				j += COLOR_GRADIENT_ACCURACY;
			}
			
			ColorGradient.DrawLine(0, i, hsv_size_x, i, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Math.Lerp(0, 360, y_value), 100, 100, 255));
			ColorLightnesss.DrawLine(0, i, hsl_size_x, i, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Hue, Math.Lerp(0, 100, y_value), 100, 255));
			i += COLOR_GRADIENT_ACCURACY;
		}
	}
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