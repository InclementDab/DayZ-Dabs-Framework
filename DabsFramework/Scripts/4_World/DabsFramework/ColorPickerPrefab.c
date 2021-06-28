// this will need to be an int if its RGBA
class ColorPickerPrefab: PrefabBase<vector>
{
	CanvasWidget HSVColorGradiant;
	CanvasWidget ColorSpectrumGradiant;
	
	Widget LastSavedColorVisual0;
		
	protected float m_CurrentHue;
	
	void ColorPickerPrefab(string caption, Class binding_context, string binding_name)
	{
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
				LastSavedColorVisual0.SetColor(ARGBF(1.0, rgb[0], rgb[1], rgb[2]));
				break;
			}
			
			case ColorSpectrumGradiant: {
				SetHue(Math.Lerp(0, 360, y_p));
				break;
			}
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
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
		
		for (int i = start_y; i < start_y + size_y; i++) {
			float y_value = (i - start_y) / size_y;
			
			for (int j = start_x; j < start_x + size_x; j++) {
				float x_value = (j - start_x) / size_x;	
				
				DFMath.HSVtoRGB(m_CurrentHue, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), rgb);
				HSVColorGradiant.DrawLine(i, j, i + 1, j + 1, 1, ARGBF(1, rgb[0], rgb[1], rgb[2]));
			}
			
			DFMath.HSVtoRGB(Math.Lerp(0, 360, y_value), 100, 100, rgb);
			ColorSpectrumGradiant.DrawLine(0, i, 0 + hsv_size_x, i, 1, ARGBF(1.0, rgb[0], rgb[1], rgb[2]));
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
		
	override string GetLayoutFile()
	{
		return "DabsFramework/GUI/layouts/prefabs/ColorPickerPrefab.layout";
	}
}


ref ColorPickerPrefab m_ColorPickerPrefabTest;
