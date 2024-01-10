class OptionSelectorColorViewController: ViewController
{	
	int Value;
	
	int Alpha, Red, Green, Blue;
	float Hue, Saturation, Var;
	
	ScriptCaller OnValueChanged;
	
	override void PropertyChanged(string property_name)
	{
		Alpha 	= Math.Clamp(Alpha, 0, 255);
		Red 	= Math.Clamp(Red, 0, 255);
		Green 	= Math.Clamp(Green, 0, 255);
		Blue 	= Math.Clamp(Blue, 0, 255);
		
		Hue 	= Math.Clamp(Hue, 0, 360);
		Saturation = Math.Clamp(Saturation, 0, 100);
		Var 	= Math.Clamp(Var, 0, 100);
		
		switch (property_name) {
			
			case "Alpha":		
			case "Red":
			case "Green":
			case "Blue": {
				Value = ARGB(Alpha, Red, Green, Blue);
				NotifyPropertyChanged("Value");
				break;
			}
			
			case "Hue": 
			case "Saturation": 
			case "Var": {
				Value = HSVtoARGB(Hue, Saturation, Var, Alpha);				
				NotifyPropertyChanged("Value");
				break;
			}
			
			case "Value": {
				InverseARGB(Value, Alpha, Red, Green, Blue);
				RGBtoHSV(Red, Green, Blue, Hue, Saturation, Var);
				NotifyPropertiesChanged({"Alpha", "Red", "Green", "Blue", "Hue", "Saturation", "Var"}, false);
				
				if (OnValueChanged) {
					OnValueChanged.Invoke();
				}
				
				break;
			}
		
		}
		// Order matters here
		super.PropertyChanged(property_name);
	}
}

class OptionSelectorColorView: OptionSelectorViewBase
{
	static const int COLOR_GRADIENT_ACCURACY = 3;
	
	CanvasWidget ColorPicker, ColorGradient, ColorLightness;
		
	Widget ColorLightnessPicker, ColorGradientPicker, ColorPickerSelector;	
	
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

		m_StartValue = m_OptionSelectorColorViewController.Value;
	}
	
	override void Update(float dt)
	{
		super.Update(dt);
				
		float size_x, size_y;
		ColorPicker.GetScreenSize(size_x, size_y);
				
		float hsv_size_x, hsv_size_y;
		ColorGradient.GetScreenSize(hsv_size_x, hsv_size_y);
		
		float hsl_size_x, hsl_size_y;
		ColorLightness.GetScreenSize(hsl_size_x, hsl_size_y);
		
		ColorPicker.Clear();
		ColorGradient.Clear();
		ColorLightness.Clear();
		
		for (int i = 0; i <= size_y; ) {
			float y_value = i / size_y;
			
			for (int j = 0; j <= size_x; ) {
				float x_value = j / size_x;	
				ColorPicker.DrawLine(i, j, i + COLOR_GRADIENT_ACCURACY, j + COLOR_GRADIENT_ACCURACY, COLOR_GRADIENT_ACCURACY, HSVtoARGB(m_OptionSelectorColorViewController.Hue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), m_OptionSelectorColorViewController.Alpha));
				j += COLOR_GRADIENT_ACCURACY;
			}
			
			ColorGradient.DrawLine(0, i, hsv_size_x, i, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Math.Lerp(0, 360, y_value), 100, 100, 255));
			ColorLightness.DrawLine(0, i, hsl_size_x, i, COLOR_GRADIENT_ACCURACY, HSVtoARGB(m_OptionSelectorColorViewController.Hue, Math.Lerp(0, 100, y_value), 100, 255));
			i += COLOR_GRADIENT_ACCURACY;
		}
		
		// Update relevant widgets to the position they are set to
		SetWidgetPosRelativeToParent(ColorPickerSelector, m_OptionSelectorColorViewController.Saturation / 100, Math.Lerp(1, 0, m_OptionSelectorColorViewController.Var / 100));
		SetWidgetPosRelativeToParent(ColorGradientPicker, 0.5, Math.InverseLerp(0, 360, m_OptionSelectorColorViewController.Hue));
		SetWidgetPosRelativeToParent(ColorLightnessPicker, 0.5, Math.InverseLerp(0, 100, m_OptionSelectorColorViewController.Saturation));
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
	
	static void GetWidgetLocalPositionNormalized(notnull Widget w, inout float x, inout float y)
	{		
		float x_pos, y_pos;
		w.GetScreenPos(x_pos, y_pos);
		x -= x_pos; y -= y_pos;
		
		float x_size, y_size;
		w.GetScreenSize(x_size, y_size);
		x /= x_size; y /= y_size;
	}
	
	static void SetWidgetPosRelativeToParent(notnull Widget w, float x, float y)
	{		
		x = Math.Clamp(x, 0, 1);
		y = Math.Clamp(y, 0, 1);
		
		Widget parent = w.GetParent();
		
		float p_x_size, p_y_size;
		parent.GetScreenSize(p_x_size, p_y_size);
		
		float x_size, y_size;
		w.GetScreenSize(x_size, y_size);
		w.SetPos((p_x_size * x) - (x_size / 2), (p_y_size * y) - (y_size / 2));
	}
	
	static void GetWidgetPosRelativeToParent(notnull Widget w, out float x, out float y)
	{		
		Widget parent = w.GetParent();
		
		float p_x_size, p_y_size;
		parent.GetScreenSize(p_x_size, p_y_size);
		if (p_x_size == 0 || p_y_size == 0) {
			return;
		}
		
		float x_pos, y_pos;
		w.GetPos(x_pos, y_pos);
		
		float x_size, y_size;
		w.GetScreenSize(x_size, y_size);
		
		x = (x_pos / p_x_size) + ((x_size / 2) / p_x_size);
		y = (y_pos / p_y_size) + ((y_size / 2) / p_y_size);
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