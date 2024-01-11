class OptionSelectorColorView: OptionSelectorViewBase
{
	static const int COLOR_GRADIENT_ACCURACY = 2;
	
	CanvasWidget ColorPicker, ColorGradient, ColorLightness;
		
	Widget ColorLightnessPicker, ColorGradientPicker, ColorPickerSelector;	
	
	SliderWidget RedSlider, GreenSlider, BlueSlider, AlphaSlider;
	EditBoxWidget RedEditBox, GreenEditBox, BlueEditBox, AlphaEditBox;
	TextWidget RedLabel, GreenLabel, BlueLabel, AlphaLabel;
	
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
		
		if ((GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) && GetGame().GetInput().HasGameFocus()) {
			int x, y;
			GetMousePos(x, y);
			float x_p = x;
			float y_p = y;
			Widget widget_under_cursor = GetWidgetUnderCursor();
			GetWidgetLocalPositionNormalized(widget_under_cursor, x_p, y_p);

			switch (widget_under_cursor) {
				case ColorPicker: {
					m_OptionSelectorColorViewController.Value = HSVtoARGB(m_OptionSelectorColorViewController.Hue, Math.Lerp(0, 100, x_p), Math.Lerp(100, 0, y_p), m_OptionSelectorColorViewController.Alpha);
					m_OptionSelectorColorViewController.NotifyPropertyChanged("Value");
					break;
				}
				
				case ColorGradient: {
					m_OptionSelectorColorViewController.Hue = Math.Lerp(0, 360, y_p);
					m_OptionSelectorColorViewController.NotifyPropertyChanged("Hue");
					break;
				}
				
				case ColorLightness: {
					m_OptionSelectorColorViewController.Saturation = Math.Lerp(0, 100, y_p);
					m_OptionSelectorColorViewController.NotifyPropertyChanged("Saturation");
					break;
				}
			}
		}
		
		// Update relevant widgets to the position they are set to
		SetWidgetPosRelativeToParent(ColorPickerSelector, m_OptionSelectorColorViewController.Saturation / 100, Math.Lerp(1, 0, m_OptionSelectorColorViewController.Var / 100));
		SetWidgetPosRelativeToParent(ColorGradientPicker, 0.5, Math.InverseLerp(0, 360, m_OptionSelectorColorViewController.Hue));
		SetWidgetPosRelativeToParent(ColorLightnessPicker, 0.5, Math.InverseLerp(0, 100, m_OptionSelectorColorViewController.Saturation));
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
	
	override string GetLayoutFile()
	{
		return "DabsFramework\\GUI\\layouts\\options\\option_types\\option_color.layout";
	}
	
	override typename GetControllerType()
	{
		return OptionSelectorColorViewController;
	}
}