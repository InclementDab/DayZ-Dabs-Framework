class ColorPickerPrefab: PrefabBase<int>
{
	static const int STEP_SIZE = 3;
	
	protected ColorPickerController m_ColorPickerController;
	
	CanvasWidget HSVColorGradient;
	CanvasWidget ColorSpectrumGradient;
	CanvasWidget ColorLightnessGradient;
	
	Widget HSVColorPickerIcon;
	Widget ColorSpectrumPickerPanel;
	Widget ColorLightnessPickerPanel;
	
	Widget CurrentColorVisual;
	Widget LastSavedColorVisual;
	
	Widget AlphaSliderFrame;
	
	void ColorPickerPrefab(string caption, Class binding_context, string binding_name, bool allow_alpha = true)
	{		
		m_ColorPickerController = ColorPickerController.Cast(GetController());

		m_ColorPickerController.LastValue = m_ColorPickerController.Value;
		m_ColorPickerController.NotifyPropertyChanged("LastValue");
		
		// order matters
		m_ColorPickerController.NotifyPropertiesChanged({"Value", "Red", "Green", "Blue", "Hue", "Saturation", "Var"});
		
		AlphaSliderFrame.Show(allow_alpha);
	}
	
	void ~ColorPickerPrefab()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(RunDrag);
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		x += 6;
		
		float x_p = x;
		float y_p = y;
		GetWidgetLocalPositionNormalized(w, x_p, y_p);
					
		switch (w) {
			case HSVColorGradient: {
				m_ColorPickerController.Value = HSVtoARGB(m_ColorPickerController.Hue, Math.Lerp(0, 100, x_p), Math.Lerp(100, 0, y_p), m_ColorPickerController.Alpha);
				m_ColorPickerController.NotifyPropertyChanged("Value");
				
				StartDragging(HSVColorGradient);
				break;
			}
			
			case ColorSpectrumGradient: {
				m_ColorPickerController.Hue = Math.Lerp(0, 360, y_p);
				m_ColorPickerController.NotifyPropertyChanged("Hue");
				
				StartDragging(ColorSpectrumGradient);
				break;
			}
			
			case ColorLightnessGradient: {
				m_ColorPickerController.Saturation = Math.Lerp(0, 100, y_p);
				m_ColorPickerController.NotifyPropertyChanged("Saturation");
				
				StartDragging(ColorLightnessGradient);
				break;
			}
			
			case CurrentColorVisual: {
				GetGame().CopyToClipboard(m_ColorPickerController.Value.ToString());
				break;
			}
			
			case LastSavedColorVisual: {
				m_ColorPickerController.Value = m_ColorPickerController.LastValue;
				m_ColorPickerController.NotifyPropertyChanged("Value");
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
		
	void StartDragging(Widget w)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(RunDrag, 0, true, w);
	}
	
	void RunDrag(Widget drag_target)
	{
		if (!(GetMouseState(MouseState.LEFT) & MB_PRESSED_MASK) || !GetGame().GetInput().HasGameFocus()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Remove(RunDrag);
			return;
		}
		
		int x, y;
		GetMousePos(x, y);
		float x_p = x;
		float y_p = y;
		GetWidgetLocalPositionNormalized(drag_target, x_p, y_p);

		switch (drag_target) {
			case HSVColorGradient: {
				m_ColorPickerController.Value = HSVtoARGB(m_ColorPickerController.Hue, Math.Lerp(0, 100, x_p), Math.Lerp(100, 0, y_p), m_ColorPickerController.Alpha);
				m_ColorPickerController.NotifyPropertyChanged("Value");
				break;
			}
			
			case ColorSpectrumGradient: {
				m_ColorPickerController.Hue = Math.Lerp(0, 360, y_p);
				m_ColorPickerController.NotifyPropertyChanged("Hue");
				break;
			}
			
			case ColorLightnessGradient: {
				m_ColorPickerController.Saturation = Math.Lerp(0, 100, y_p);
				m_ColorPickerController.NotifyPropertyChanged("Saturation");
				break;
			}
		}
	}
		
	override void PrefabPropertyChanged(string property_name)
	{
		super.PrefabPropertyChanged(property_name);
		
		switch (property_name) {
			case "Value": {
				SetWidgetPosRelativeToParent(HSVColorPickerIcon, m_ColorPickerController.Saturation / 100, Math.Lerp(1, 0, m_ColorPickerController.Var / 100));
				SetWidgetPosRelativeToParent(ColorSpectrumPickerPanel, 0.5, Math.InverseLerp(0, 360, m_ColorPickerController.Hue));
				SetWidgetPosRelativeToParent(ColorLightnessPickerPanel, 0.5, Math.InverseLerp(0, 100, m_ColorPickerController.Saturation));
				UpdateHSVSpectrum();
				break;
			}
			
			case "Hue": {	
				float x, y;
				GetWidgetPosRelativeToParent(HSVColorPickerIcon, x, y);
				m_ColorPickerController.Value = HSVtoARGB(m_ColorPickerController.Hue, Math.Lerp(0, 100, x), Math.Lerp(100, 0, y), m_ColorPickerController.Alpha);
				m_ColorPickerController.NotifyPropertyChanged("Value", false);				
				UpdateHSVSpectrum();
				break;
			}
		}
	}
	
	void UpdateHSVSpectrum()
	{				
		float size_x, size_y;
		HSVColorGradient.GetScreenSize(size_x, size_y);
				
		float hsv_size_x, hsv_size_y;
		ColorSpectrumGradient.GetScreenSize(hsv_size_x, hsv_size_y);
		
		float hsl_size_x, hsl_size_y;
		ColorLightnessGradient.GetScreenSize(hsl_size_x, hsl_size_y);
		
		HSVColorGradient.Clear();
		ColorSpectrumGradient.Clear();
		ColorLightnessGradient.Clear();
		
		for (int i = 0; i <= size_y; ) {
			float y_value = i / size_y;
			
			for (int j = 0; j <= size_x; ) {
				float x_value = j / size_x;	
				HSVColorGradient.DrawLine(i, j, i + STEP_SIZE, j + STEP_SIZE, STEP_SIZE, HSVtoARGB(m_ColorPickerController.Hue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), m_ColorPickerController.Alpha));
				j += STEP_SIZE;
			}
			
			ColorSpectrumGradient.DrawLine(0, i, hsv_size_x, i, STEP_SIZE, HSVtoARGB(Math.Lerp(0, 360, y_value), 100, 100, 255));
			ColorLightnessGradient.DrawLine(0, i, hsl_size_x, i, STEP_SIZE, HSVtoARGB(m_ColorPickerController.Hue, Math.Lerp(0, 100, y_value), 100, 255));
			i += STEP_SIZE;
		}
	}
		
	static void GetWidgetLocalPositionNormalized(Widget w, inout float x, inout float y)
	{
		if (!w) {
			return;
		}
		
		float x_pos, y_pos;
		w.GetScreenPos(x_pos, y_pos);
		x -= x_pos; y -= y_pos;
		
		float x_size, y_size;
		w.GetScreenSize(x_size, y_size);
		x /= x_size; y /= y_size;
	}
	
	static void SetWidgetPosRelativeToParent(Widget w, float x, float y)
	{
		if (!w) {
			return;
		}
		
		x = Math.Clamp(x, 0, 1);
		y = Math.Clamp(y, 0, 1);
		
		Widget parent = w.GetParent();
		
		float p_x_size, p_y_size;
		parent.GetScreenSize(p_x_size, p_y_size);
		
		float x_size, y_size;
		w.GetScreenSize(x_size, y_size);
		w.SetPos((p_x_size * x) - (x_size / 2), (p_y_size * y) - (y_size / 2));
	}
	
	static void GetWidgetPosRelativeToParent(Widget w, out float x, out float y)
	{
		if (!w) {
			return;
		}
		
		Widget parent = w.GetParent();
		
		float p_x_size, p_y_size;
		parent.GetScreenSize(p_x_size, p_y_size);
		if (p_x_size == 0 || p_y_size == 0) {
			// div by zero
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
		return "DabsFramework/GUI/layouts/prefabs/ColorPickerPrefab.layout";
	}
			
	override typename GetControllerType() 
	{
		return ColorPickerController;
	}
}