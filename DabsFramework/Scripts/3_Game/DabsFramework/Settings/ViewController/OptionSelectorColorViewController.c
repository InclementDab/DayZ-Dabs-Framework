class OptionSelectorColorViewController: ViewController
{	
	static const int COLOR_GRADIENT_ACCURACY = 6;
	
	int Value, StartValue;
	
	int Alpha, Red, Green, Blue;
	string AlphaUserInput, RedUserInput, GreenUserInput, BlueUserInput;
	float Hue, Saturation, Var;
	
	ScriptCaller OnValueChanged;
	ButtonWidget SelectButton, CancelButton;
	
	CanvasWidget ColorPicker, ColorGradient, ColorLightness, ColorPickerPanel;
	Widget ColorLightnessPicker, ColorGradientPicker, ColorPickerSelector, ColorPickerWrapper;	
	
	void UpdateVisuals()
	{
		if (!ColorPickerWrapper.IsVisible()) {
			return;
		}
		
		float size_x, size_y;
		ColorPicker.GetScreenSize(size_x, size_y);
				
		float hsv_size_x, hsv_size_y;
		ColorGradient.GetScreenSize(hsv_size_x, hsv_size_y);
		
		float hsl_size_x, hsl_size_y;
		ColorLightness.GetScreenSize(hsl_size_x, hsl_size_y);
		
		ColorPicker.Clear();
		ColorGradient.Clear();
		ColorLightness.Clear();
		SelectButton.SetColor(Value);
		CancelButton.SetColor(StartValue);
		
		WidgetAnimator.AnimateColor(ColorPickerPanel, Value, 10);
		
		for (int i = 0; i <= size_y; ) {
			float y_value = i / size_y;
			
			for (int j = 0; j <= size_x; ) {
				float x_value = j / size_x;	
				ColorPicker.DrawLine(i, j, i + COLOR_GRADIENT_ACCURACY, j + COLOR_GRADIENT_ACCURACY, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Hue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), Alpha));
				j += COLOR_GRADIENT_ACCURACY;
			}
			
			ColorGradient.DrawLine(0, i, hsv_size_x, i, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Math.Lerp(0, 360, y_value), 100, 100, 255));
			ColorLightness.DrawLine(0, i, hsl_size_x, i, COLOR_GRADIENT_ACCURACY, HSVtoARGB(Hue, Math.Lerp(0, 100, y_value), 100, 255));
			i += COLOR_GRADIENT_ACCURACY;
		}
	
		
		// Update relevant widgets to the position they are set to
		SetWidgetPosRelativeToParent(ColorPickerSelector, Saturation / 100, Math.Lerp(1, 0, Var / 100));
		SetWidgetPosRelativeToParent(ColorGradientPicker, 0.5, Math.InverseLerp(0, 360, Hue));
		SetWidgetPosRelativeToParent(ColorLightnessPicker, 0.5, Math.InverseLerp(0, 100, Saturation));
	}
		
	protected void DoCursorDrag(Widget drag_target)
	{
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) || !GetGame().GetInput().HasGameFocus()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(DoCursorDrag);
			return;
		}
		
		Widget widget_under_cursor = GetWidgetUnderCursor();
		if (widget_under_cursor != drag_target) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(DoCursorDrag);
			return;
		}
		
		int x, y;
		GetMousePos(x, y);
		float x_p = x;
		float y_p = y;
		GetWidgetLocalPositionNormalized(widget_under_cursor, x_p, y_p);

		switch (widget_under_cursor) {
			case ColorPicker: {
				Value = HSVtoARGB(Hue, Math.Lerp(0, 100, x_p), Math.Lerp(100, 0, y_p), Alpha);
				NotifyPropertyChanged("Value");
				break;
			}
			
			case ColorGradient: {
				Hue = Math.Lerp(0, 360, y_p);
				NotifyPropertyChanged("Hue");
				break;
			}
			
			case ColorLightness: {
				Saturation = Math.Lerp(0, 100, y_p);
				NotifyPropertyChanged("Saturation");
				break;
			}
		}
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		switch (w) {
			case ColorPicker: 
			case ColorGradient: 
			case ColorLightness: {
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(DoCursorDrag, 0, true, w);
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
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
				
				AlphaUserInput = Alpha.ToString();
				RedUserInput = Red.ToString();
				GreenUserInput = Green.ToString();
				BlueUserInput = Blue.ToString();
								
				NotifyPropertiesChanged({"Alpha", "Red", "Green", "Blue", "Hue", "Saturation", "Var", "AlphaUserInput", "RedUserInput", "GreenUserInput", "BlueUserInput"}, false);
				
				UpdateVisuals();
				
				if (OnValueChanged) {
					OnValueChanged.Invoke();
				}
				
				break;
			}
			
			// Force user input to be alpha numeric
			case "AlphaUserInput": 
			case "RedUserInput":
			case "GreenUserInput":
			case "BlueUserInput": {
				EnScript.SetClassVar(this, property_name, 0, Math.Clamp(EnScriptVar<string>.Get(this, property_name).ToInt(), 0, 255).ToString());
				NotifyPropertyChanged(property_name, false);
				break;
			}
		}		
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
}