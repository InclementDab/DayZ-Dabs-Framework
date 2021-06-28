// this will need to be an int if its RGBA
class ColorPickerPrefab: PrefabBase<vector>
{
	CanvasWidget HSVColorGradiant;
	CanvasWidget ColorSpectrumGradiant;
	
	void ColorPickerPrefab(string caption, Class binding_context, string binding_name)
	{
		
		float rgb[3];
		
		int start_x = 0;
		int start_y = 0;
		
		float size_x, size_y;
		HSVColorGradiant.GetScreenSize(size_x, size_y);
		Print(size_x);
		Print(size_y);
		
		int width_hsv_x = size_x * 0.1;
		int offset_hsv_x = 10;
		
		HSVColorGradiant.Clear();
		ColorSpectrumGradiant.Clear();
		
		int hsv_x = start_x + size_x + offset_hsv_x;
		for (int i = start_y; i < start_y + size_y; i++) {
			float y_value = (i - start_y) / size_y;
			
			for (int j = start_x; j < start_x + size_x; j++) {
				float x_value = (j - start_x) / size_x;	
				
				DFMath.HSVtoRGB(0, Math.Lerp(0, 100, y_value), Math.Lerp(100, 0, x_value), rgb);	
				HSVColorGradiant.DrawLine(i, j, i + 1, j + 1, 1, ARGBF(1, rgb[0], rgb[1], rgb[2]));
			}
			
			//EditorMath.HSVtoRGB(Math.Lerp(0, 360, y_value), 100, 100, rgb);
			//canvas.DrawLine(hsv_x, i, hsv_x + width_hsv_x, i, 1, ARGBF(1.0, rgb[0], rgb[1], rgb[2]));
		}

	}
	
	override bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		
		if (w == HSVColorGradiant) {
			Print("A");
		}
		
		return super.OnMouseButtonDown(w, x, y, button);
	}
	
	override string GetLayoutFile()
	{
		return "DabsFramework/GUI/layouts/prefabs/ColorPickerPrefab.layout";
	}
}


ref ColorPickerPrefab m_ColorPickerPrefabTest;
