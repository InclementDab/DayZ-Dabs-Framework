class ColorPickerPrefab: PrefabBase<int>
{
	static const int STEP_SIZE = 3;
	
	CanvasWidget HSVColorGradiant;
	CanvasWidget ColorSpectrumGradiant;
	
	Widget HSVColorPickerIcon;
			
	protected float m_CurrentHue;
	
	void ColorPickerPrefab(string caption, Class binding_context, string binding_name)
	{
		int a, r, g, b;
		InverseARGB(m_PrefabBaseController.Value, a, r, g, b);
		m_CurrentHue = DFMath.RGBtoHue(r, g, b);
		
		UpdateHSVSpectrum();
	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		float x_p = x;
		float y_p = y;
		GetWidgetLocalPositionNormalized(w, x_p, y_p);
					
		float rgb[3];		
		switch (w) {
			case HSVColorGradiant: {
				DFMath.HSVtoRGB(m_CurrentHue, Math.Lerp(0, 100, x_p), Math.Lerp(100, 0, y_p), rgb);
				m_PrefabBaseController.Value = ARGBF(1.0, rgb[0], rgb[1], rgb[2]);
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
		switch (property_name) {
			case "Value": {
				float a, r, g, b, h, s, v;
				InverseARGBF(m_PrefabBaseController.Value, a, r, g, b);
				DFMath.RGBtoHSV(r, g, b, h, s, v);
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
	}
	
	void UpdateHSVSpectrum()
	{
		float rgb[3];
		
		int start_x = 0;
		int start_y = 0;
		
		float size_x, size_y;
		HSVColorGradiant.GetScreenSize(size_x, size_y);
				
		float hsv_size_x, hsv_size_y;
		ColorSpectrumGradiant.GetScreenSize(hsv_size_x, hsv_size_y);
		
		HSVColorGradiant.Clear();
		ColorSpectrumGradiant.Clear();
		
		for (int i = start_y; i < start_y + size_y; ) {
			float y_value = (i - start_y) / size_y;
			
			for (int j = start_x; j < start_x + size_x; ) {
				float x_value = (j - start_x) / size_x;	
				
				DFMath.HSVtoRGB(m_CurrentHue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), rgb);
				HSVColorGradiant.DrawLine(i, j, i + STEP_SIZE, j + STEP_SIZE, STEP_SIZE, ARGBF(1, rgb[0], rgb[1], rgb[2]));
				j += STEP_SIZE;
			}
			
			DFMath.HSVtoRGB(Math.Lerp(0, 360, y_value), 100, 100, rgb);
			ColorSpectrumGradiant.DrawLine(0, i, 0 + hsv_size_x, i, STEP_SIZE, ARGBF(1.0, rgb[0], rgb[1], rgb[2]));
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
		
	override string GetLayoutFile()
	{
		return "DabsFramework/GUI/layouts/prefabs/ColorPickerPrefab.layout";
	}
}