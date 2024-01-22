class WidgetAnimationTimer: Managed
{
	protected Widget m_Source;
	protected WidgetAnimatorProperty m_Property;
	protected float m_StartValue, m_EndValue;
	protected int m_Time, m_StartTime;
	protected bool m_Loop;

	protected ScriptInvoker m_UpdateQueue = GetGame().GetUpdateQueue(CALL_CATEGORY_GUI);
		
	void ~WidgetAnimationTimer()
	{
		if (m_UpdateQueue) {
			m_UpdateQueue.Remove(DoAnimate);
		}
	}

	void Run(Widget source, WidgetAnimatorProperty property, float start_value, float end_value, int time, bool loop)
	{
		m_Source = source;
		m_Property = property;
		m_StartValue = start_value;
		m_EndValue = end_value;
		m_Time = time;
		m_Loop = loop;
		m_StartTime = GetGame().GetTime();
		
		if (m_Time <= 0) {
			Error("Time out of bounds " + m_Time);
			return;
		}
		
		m_UpdateQueue.Insert(DoAnimate);
	}
	
	protected void DoAnimate()
	{
		if (!m_Source) {
			m_UpdateQueue.Remove(DoAnimate);
			delete this;
			return;
		}
		
		float normalized = ((GetGame().GetTime() - m_StartTime) / m_Time);
		normalized = Math.Clamp(normalized, 0, 1);
		
		SetProperty(m_Source, m_Property, Math.Lerp(m_StartValue, m_EndValue, normalized));
		
		if (normalized >= 1) {	
			// reset the value once we reach the end
			if (m_Loop) {
				SetProperty(m_Source, m_Property, m_StartValue);
				m_StartTime = GetGame().GetTime();
				return;
			}
			
			m_UpdateQueue.Remove(DoAnimate);

			// finally set end value to make sure it was completed
			SetProperty(m_Source, m_Property, m_EndValue);
			delete this;
		}
	}
	
	static void SetProperty(notnull Widget source, WidgetAnimatorProperty property, float value)
	{
		vector rotation;
		float pos_x, pos_y, size_w, size_h, color_a, color_r, color_g, color_b, color_h, color_s, color_v;
		int color;
		switch (property) {
			case WidgetAnimatorProperty.POSITION_X: {
				source.GetPos(pos_x, pos_y);
				source.SetPos(value, pos_y);
				break;
			}
			
			case WidgetAnimatorProperty.POSITION_Y: {
				source.GetPos(pos_x, pos_y);
				source.SetPos(pos_x, value);
				break;
			}
			
			case WidgetAnimatorProperty.SIZE_W: {
				source.GetSize(size_w, size_h);
				source.SetSize(value, size_h);
				break;
			}
			
			case WidgetAnimatorProperty.SIZE_H: {
				source.GetSize(size_w, size_h);
				source.SetSize(size_w, value);
				break;
			}			
			
			case WidgetAnimatorProperty.ROTATION_X: {
				rotation = source.GetRotation();
				source.SetRotation(value, rotation[1], rotation[2]);
				break;
			}
			
			case WidgetAnimatorProperty.ROTATION_Y: {
				rotation = source.GetRotation();
				source.SetRotation(rotation[0], value, rotation[2]);
				break;
			}		
				
			case WidgetAnimatorProperty.ROTATION_Z: {
				rotation = source.GetRotation();
				source.SetRotation(rotation[0], rotation[1], value);
				break;
			}
			
			case WidgetAnimatorProperty.COLOR_A: {
				source.SetAlpha(value);
				break;
			}
			
			case WidgetAnimatorProperty.COLOR_R: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				source.SetColor(ARGBF(color_a, value, color_g, color_b));
				break;
			}
						
			case WidgetAnimatorProperty.COLOR_G: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				source.SetColor(ARGBF(color_a, color_r, value, color_b));
				break;
			}	
								
			case WidgetAnimatorProperty.COLOR_B: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				source.SetColor(ARGBF(color_a, color_r, color_g, value));
				break;
			}
			
			case WidgetAnimatorProperty.COLOR_H: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				RGBFtoHSV(color_r, color_g, color_b, color_h, color_s, color_v);
				source.SetColor(HSVtoARGB(value, color_s, color_v, color_a * 255)); // what a mess
				break;
			}	
					
			case WidgetAnimatorProperty.COLOR_S: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				RGBFtoHSV(color_r, color_g, color_b, color_h, color_s, color_v);
				source.SetColor(HSVtoARGB(color_h, value, color_v, color_a * 255));
				break;
			}		
						
			case WidgetAnimatorProperty.COLOR_V: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				RGBFtoHSV(color_r, color_g, color_b, color_h, color_s, color_v);
				source.SetColor(HSVtoARGB(color_h, color_s, value, color_a * 255));
				break;
			}
			
			case WidgetAnimatorProperty.EXACT_TEXT: {
				TextWidget tw = TextWidget.Cast(source);
				if (tw) {
					tw.SetTextExactSize(value);
				}
				break;
			}
		}
	}
	
	static float GetProperty(notnull Widget source, WidgetAnimatorProperty property)
	{
		float pos_x, pos_y, size_w, size_h, color_a, color_r, color_g, color_b;
		switch (property) {
			case WidgetAnimatorProperty.POSITION_X: {
				source.GetPos(pos_x, pos_y);
				return pos_x;
			}
			
			case WidgetAnimatorProperty.POSITION_Y: {
				source.GetPos(pos_x, pos_y);
				return pos_y;
			}			
			
			case WidgetAnimatorProperty.SIZE_W: {
				source.GetSize(size_w, size_h);
				return size_w;
			}			
			
			case WidgetAnimatorProperty.SIZE_H: {
				source.GetSize(size_w, size_h);
				return size_h;
			}			
			
			case WidgetAnimatorProperty.ROTATION_X: {
				return source.GetRotation()[0];
			}	
					
			case WidgetAnimatorProperty.ROTATION_Y: {
				return source.GetRotation()[1];
			}	
					
			case WidgetAnimatorProperty.ROTATION_Z: {
				return source.GetRotation()[2];
			}
			
			case WidgetAnimatorProperty.COLOR_A: {
				return source.GetAlpha();
			}
			
			case WidgetAnimatorProperty.COLOR_R: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				return color_r;
			}	
					
			case WidgetAnimatorProperty.COLOR_G: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				return color_g;
			}	
			
			case WidgetAnimatorProperty.COLOR_B: {
				InverseARGBF(source.GetColor(), color_a, color_r, color_g, color_b);
				return color_b;
			}			
			
			// i dont feel like doing hsv implementation rn its dummy annoying
		}
		
		Error("Unsupported WidgetAnimatorProperty");
		return 0;
	}
	
	Widget GetWidget()
	{
		return m_Source;
	}
	
	WidgetAnimatorProperty GetWidgetAnimatorProperty()
	{
		return m_Property;
	}

	float GetStartValue()
	{
		return m_StartValue;
	}

	float GetEndValue()
	{
		return m_EndValue;
	}
}