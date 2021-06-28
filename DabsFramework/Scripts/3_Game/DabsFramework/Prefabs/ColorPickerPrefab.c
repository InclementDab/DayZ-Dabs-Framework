class ColorPickerController: PrefabBaseController<int>
{
	int Red, Green, Blue;
	
	override void PropertyChanged(string property_name)
	{
		switch (property_name) {
						
			case "Red":
			case "Green":
			case "Blue": {
				Value = ARGB(255, Red, Green, Blue);
				super.PropertyChanged("Value");
				break;
			}
		}
	}
}

class ColorPickerPrefab: PrefabBase<int>
{
	static const int STEP_SIZE = 4;
	
	protected ColorPickerController m_ColorPickerController;
	
	CanvasWidget HSVColorGradiant;
	CanvasWidget ColorSpectrumGradiant;
	
	Widget HSVColorPickerIcon;
			
	protected float m_CurrentHue;
	
	void ColorPickerPrefab(string caption, Class binding_context, string binding_name)
	{		
		m_ColorPickerController = ColorPickerController.Cast(GetController());
		
		int a, r, g, b;
		InverseARGB(m_PrefabBaseController.Value, a, r, g, b);
		m_CurrentHue = DFMath.RGBtoHue(r, g, b);
		
		UpdateHSVSpectrum();
		
		m_PrefabBaseController.NotifyPropertyChanged("Value");
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		float x_p = x;
		float y_p = y;
		GetWidgetLocalPositionNormalized(w, x_p, y_p);
					
		switch (w) {
			case HSVColorGradiant: {
				m_PrefabBaseController.Value = HSVtoRGB(m_CurrentHue, Math.Lerp(0, 100, x_p), Math.Lerp(100, 0, y_p));
				m_PrefabBaseController.NotifyPropertyChanged("Value");
				break;
			}
			
			case ColorSpectrumGradiant: {
				SetHue(Math.Lerp(0, 360, y_p));
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		super.PrefabPropertyChanged(property_name);
		
		switch (property_name) {
			case "Value": {
				float a, r, g, b, h, s, v;
				InverseARGBF(m_PrefabBaseController.Value, a, r, g, b);
				RGBFtoHSV(r, g, b, h, s, v);
				SetHue(h);
				SetWidgetPosRelativeToParent(HSVColorPickerIcon, s / 100, Math.Lerp(1, 0, v / 100));
				break;
			}
		}
	}
	
	void SetHue(float hue)
	{
		m_CurrentHue = hue;
		UpdateHSVSpectrum();
		
		// Update active color
		float x, y;
		GetWidgetPosRelativeToParent(HSVColorPickerIcon, x, y);
		m_PrefabBaseController.Value = HSVtoRGB(m_CurrentHue, Math.Lerp(0, 100, x), Math.Lerp(100, 0, y));
		m_PrefabBaseController.NotifyPropertyChanged("Value", false);
	}
	
	void UpdateHSVSpectrum()
	{				
		float size_x, size_y;
		HSVColorGradiant.GetScreenSize(size_x, size_y);
				
		float hsv_size_x, hsv_size_y;
		ColorSpectrumGradiant.GetScreenSize(hsv_size_x, hsv_size_y);
		
		HSVColorGradiant.Clear();
		ColorSpectrumGradiant.Clear();
		
		for (int i = 0; i <= size_y; ) {
			float y_value = i / size_y;
			
			for (int j = 0; j <= size_x; ) {
				float x_value = j / size_x;	
				HSVColorGradiant.DrawLine(i, j, i + STEP_SIZE, j + STEP_SIZE, STEP_SIZE, HSVtoRGB(m_CurrentHue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value)));
				j += STEP_SIZE;
			}
			
			ColorSpectrumGradiant.DrawLine(0, i, 0 + hsv_size_x, i, STEP_SIZE, HSVtoRGB(Math.Lerp(0, 360, y_value), 100, 100));
			i += STEP_SIZE;
		}
	}
		
	void GetWidgetLocalPositionNormalized(Widget w, inout float x, inout float y)
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
	
	void SetWidgetPosRelativeToParent(Widget w, float x, float y)
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
	
	void GetWidgetPosRelativeToParent(Widget w, out float x, out float y)
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